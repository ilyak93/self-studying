#include "GreetingServer.hpp"
#include "VotesMap.hpp"
#include "Vote.hpp"
#include "gRPCObjects/paxos/FuturePaxosGreetingClient.hpp"
#include "gRPCObjects/interceptors/ServerInterceptor.hpp"
#include "ZKManager.hpp"
#include <atomic>
#include <mutex>
#include <future>
#include <iostream>
#include <Server.hpp>

GreetingServer::GreetingServer(int id, int port, const std::string& state) : id(id), state(state) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:" + std::to_string(port), grpc::InsecureServerCredentials());
    builder.RegisterService(this);
    builder.AddChannelArgument(GRPC_ARG_ENABLE_CHANNELZ, 1);

    std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> interceptor_creators;
    interceptor_creators.emplace_back(std::make_unique<ServerInterceptorFactory>(interceptorFactory));
    builder.experimental().SetInterceptorCreators(std::move(interceptor_creators));

    greetingServer = builder.BuildAndStart();
    if (!greetingServer) {
        std::cerr << "Failed to start server on port " << port << std::endl;
        exit(1);
    }
}

void GreetingServer::shutdown() {
    greetingServer->Shutdown();
}

void GreetingServer::addRestrict(const std::string& client, const std::string& procedure) {
    interceptorFactory.AddRestrict(client, procedure);
}

grpc::Status GreetingServer::ReceiveVote(grpc::ServerContext* context, const protos::VoteRequest* request, protos::VoteReply* response) {
    response->set_clientid(request->clientid());
    response->set_party(request->party());
    response->set_originstate(request->originstate());
    response->set_currentstate(request->currentstate());
    response->set_timestamp(request->timestamp());
    response->set_serverid(id);

    Vote remoteVote(request->clientid(), request->party(), request->originstate(),
                                 request->currentstate(), request->timestamp());

    if (Server::electionsEnded.load()) {
        bool insertedToVotesMap = false;
        while (!insertedToVotesMap) {
            std::lock_guard<std::mutex> lock(VotesMap::mutex);
            try {
                auto currentMapVote = VotesMap::get(request->clientid());
                std::cout << remoteVote.toString() << std::endl;
                if (!currentMapVote || request->timestamp() >= currentMapVote->getTimeStamp()) {
                    VotesMap::put(remoteVote.getClientId(), std::make_shared<Vote>(remoteVote));
                }
                insertedToVotesMap = true;
            } catch (const std::exception& e) {
                insertedToVotesMap = false;
            }
        }
    }

    if (this->state == request->originstate() && request->currentstate() != request->originstate()) {
        try {
            auto future = FuturePaxosGreetingClient().calculate(
                Server::zkManager->getCurrentStateAddressesForPaxos(),
                Server::serverId,
                remoteVote
            );
            int voteNumber = Server::votesCounter.fetch_add(1);
            Server::votesInDistributionProcess[voteNumber] = std::move(future);
        } catch (const std::exception& e) {
            std::cerr << "Exception in receiveVote remoteVote: " << e.what() << std::endl;
        }
    }

    return grpc::Status::OK;
}

grpc::Status GreetingServer::ReceiveStartElections(grpc::ServerContext* context, const protos::StartElectionsRequest* request, protos::StartElectionsReply* response) {
    Server::electionsStarted.store(true);
    return grpc::Status::OK;
}

grpc::Status GreetingServer::ReceiveEndElections(grpc::ServerContext* context, const protos::EndElectionsRequest* request, protos::EndElectionsReply* response) {
    while (!Server::electionsEnded.load()) {
        std::lock_guard<std::mutex> lock(Server::sendingRemoteVoteMutex);
        if (Server::sendingRemoteVoteCounter.load() == 0) {
            Server::electionsStarted.store(true);
            Server::electionsEnded.store(true);
            Server::zkManager->registerFinishedRemoteSending();
        }
    }
    return grpc::Status::OK;
}

grpc::Status GreetingServer::ReciveVotesCount(grpc::ServerContext* context, const protos::VotesCountForPartyRequest* request, grpc::ServerWriter<protos::VotesCountForPartyReply>* writer) {
    auto votesCounts = VotesMap::countVotes();
    for (const auto& [party, votesCount] : votesCounts) {
        protos::VotesCountForPartyReply reply;
        reply.set_state(this->state);
        reply.set_party(votesCount->getParty());
        reply.set_votescount(votesCount->getCount());
        if (!writer->Write(reply)) {
            break;
        }
    }
    return grpc::Status::OK;
}

grpc::Status GreetingServer::ReceiveStatus(grpc::ServerContext* context, const protos::VotesCountForPartyRequest* request, protos::VotesCountForPartyReply* response) {
    auto votesCounts = VotesMap::countVotes();
    auto it = votesCounts.find(request->party());
    if (it == votesCounts.end()) {
        response->set_party(request->party());
        response->set_state(request->state());
        response->set_votescount(0);
    } else {
        response->set_party(it->second->getParty());
        response->set_state(it->second->getState());
        response->set_votescount(it->second->getCount());
    }
    return grpc::Status::OK;
}