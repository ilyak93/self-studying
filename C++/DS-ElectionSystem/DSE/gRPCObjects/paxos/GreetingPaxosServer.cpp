// GreetingPaxosServer.cpp

#include "GreetingPaxosServer.hpp"
#include "gRPCObjects/interceptors/ServerInterceptor.hpp"
#include "PrepareAndPromise.hpp"
#include "SessionKey.hpp"
#include "AcceptAndAccepted.hpp"
#include "InitAndSession.hpp"
#include <iostream>

std::atomic<int> GreetingPaxosServer::sessionsCounter(0);

GreetingPaxosServer::GreetingPaxosServer(int id, int port) : id(id) {
    std::string server_address = "0.0.0.0:" + std::to_string(port);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(this);
    builder.AddChannelArgument(GRPC_ARG_ALLOW_REUSEPORT, 0);

    // Add the interceptor
    std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> interceptor_creators;
    interceptor_creators.emplace_back(new ServerInterceptorImpl());
    builder.experimental().SetInterceptorCreators(std::move(interceptor_creators));

    greetingServer = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
}

void GreetingPaxosServer::shutdown() {
    greetingServer->Shutdown();
}

grpc::Status GreetingPaxosServer::UponReceivingInit(grpc::ServerContext* context, const protos::Init* init, protos::Init* response) {
    protos::Session newSession = SessionsMap::createNewSession(init->leaderid(), this->id, init->voterid(), init->sessionid());
    SessionKey newSessionKey(newSession.sessionid(), newSession.leaderid());
    SessionsMap::put(newSessionKey, newSession);
    InitAndSession initAndSession(*init, newSession);
    *response = initAndSession.getInit();
    return grpc::Status::OK;
}

grpc::Status GreetingPaxosServer::UponReceivingPrepare(grpc::ServerContext* context, const protos::Prepare* prepare, protos::Promise* response) {
    SessionKey sessionKey(prepare->sessionid(), prepare->leaderid());
    std::shared_ptr<protos::Session> session = SessionsMap::get(sessionKey);
    PrepareAndPromise prepareAndPromise(*prepare, *session);
    SessionsMap::put(sessionKey, prepareAndPromise.getSession());
    *response = prepareAndPromise.getPromise();
    return grpc::Status::OK;
}

grpc::Status GreetingPaxosServer::UponReceivingAccept(grpc::ServerContext* context, const protos::Accept* accept, protos::Accepted* response) {
    SessionKey sessionKey(accept->sessionid(), accept->leaderid());
    std::shared_ptr<protos::Session> session = SessionsMap::get(sessionKey);
    AcceptAndAccepted acceptAndAccepted(*accept, *session);
    SessionsMap::put(sessionKey, acceptAndAccepted.getSession());
    *response = acceptAndAccepted.getAccepted();
    return grpc::Status::OK;
}

grpc::Status GreetingPaxosServer::UponReceivingCommit(grpc::ServerContext* context, const protos::Commit* commit, protos::Finish* response) {
    Vote commitedVote(commit->vote().clientid(),
                      commit->vote().party(),
                      commit->vote().originstate(),
                      commit->vote().currentstate(),
                      commit->vote().timestamp());
    
    bool insertedToVotesMap = false;
    while(!insertedToVotesMap) {
        std::lock_guard<std::mutex> lock(VotesMap::mutex);
        try {
            std::shared_ptr<Vote> currentVoteInMap = VotesMap::get(commitedVote.getClientId());
            std::cout << commitedVote.toString() << std::endl;
            if(currentVoteInMap == nullptr || commitedVote.getTimeStamp() >= currentVoteInMap->getTimeStamp()) {
                VotesMap::put(commitedVote.getClientId(), std::make_shared<Vote>(commitedVote));
            }
            insertedToVotesMap = true;
        } catch(const std::exception& e) {
            insertedToVotesMap = false;
        }
    }

    SessionKey sessionKey(commit->sessionid(), commit->leaderid());
    SessionsMap::remove(sessionKey);
    
    // Finish doesn't have any fields, so we don't need to set anything
    return grpc::Status::OK;
}