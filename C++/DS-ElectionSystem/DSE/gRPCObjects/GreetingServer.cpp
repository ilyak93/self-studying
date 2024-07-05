#include "GreetingServer.hpp"
#include "VotesMap.hpp"
#include "Vote.hpp"
#include "FuturePaxosGreetingClient.hpp"
#include "gRPCObjects/interceptors/ServerInterceptor.hpp"
#include "ZKManager.hpp"
#include <atomic>
#include <mutex>
#include <future>
#include <iostream>

GreetingServer::GreetingServer(int id, int port, const std::string& state) : id(id), state(state) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:" + std::to_string(port), grpc::InsecureServerCredentials());
    builder.RegisterService(this);
    builder.AddChannelArgument(GRPC_ARG_ENABLE_CHANNELZ, 1);

    std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> interceptor_creators;
    interceptor_creators.emplace_back(std::make_unique<ServerInterceptorImpl::Factory>());
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

grpc::Status GreetingServer::ReceiveVote(grpc::ServerContext* context, const protos::VoteRequest* request, protos::VoteReply* response) {
    response->set_clientid(request->clientid());
    response->set_party(request->party());
    response->set_originstate(request->originstate());
    response->set_currentstate(request->currentstate());
    response->set_timestamp(request->timestamp());
    response->set_serverid(id);

    Vote remoteVote(request->clientid(), request->party(), request->originstate(),
                                 request->currentstate(), request->timestamp());

    if (app::Server::electionsEnded.load()) {
        bool insertedToVotesMap = false;
        while (!insertedToVotesMap) {
            std::lock_guard<std::mutex> lock(VotesMap::mutex);
            try {
                auto currentMapVote = VotesMap::get(request->clientid());
                std::cout << remoteVote.toString() << std::endl;
                if (!currentMapVote || request->timestamp() >= currentMapVote->getTimeStamp()) {
                    VotesMap::put(remoteVote.getClientId(), remoteVote);
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
                app::Server::zkManager->getCurrentStateAddressesForPaxos(),
                app::Server::serverId,
                remoteVote
            );
            int voteNumber = app::Server::votesCounter.fetch_add(1);
            app::Server::votesInDistributionProcess[voteNumber] = std::move(future);
        } catch (const std::exception& e) {
            std::cerr << "Exception in receiveVote remoteVote: " << e.what() << std::endl;
        }
    }

    return grpc::Status::OK;
}

grpc::Status GreetingServer::receiveStartElections(grpc::ServerContext* context, const protos::StartElectionsRequest* request, protos::StartElectionsReply* response) {
    app::Server::electionsStarted.store(true);
    return grpc::Status::OK;
}

grpc::Status GreetingServer::receiveEndElections(grpc::ServerContext* context, const protos::EndElectionsRequest* request, protos::EndElectionsReply* response) {
    while (!app::Server::electionsEnded.load()) {
        std::lock_guard<std::mutex> lock(app::Server::sendingRemoteVoteMutex);
        if (app::Server::sendingRemoteVoteCounter.load() == 0) {
            app::Server::electionsStarted.store(true);
            app::Server::electionsEnded.store(true);
            app::Server::zkManager->registerFinishedRemoteSending();
        }
    }
    return grpc::Status::OK;
}

grpc::Status GreetingServer::reciveVotesCount(grpc::ServerContext* context, const protos::VotesCountForPartyRequest* request, grpc::ServerWriter<protos::VotesCountForPartyReply>* writer) {
    auto votesCounts = VotesMap::countVotes();
    for (const auto& [party, votesCount] : votesCounts) {
        protos::VotesCountForPartyReply reply;
        reply.set_state(this->state);
        reply.set_party(votesCount.getParty());
        reply.set_votescount(votesCount.getCount());
        if (!writer->Write(reply)) {
            break;
        }
    }
    return grpc::Status::OK;
}

grpc::Status GreetingServer::receiveStatus(grpc::ServerContext* context, const protos::VotesCountForPartyRequest* request, protos::VotesCountForPartyReply* response) {
    auto votesCounts = VotesMap::countVotes();
    auto it = votesCounts.find(request->party());
    if (it == votesCounts.end()) {
        response->set_party(request->party());
        response->set_state(request->state());
        response->set_votescount(0);
    } else {
        response->set_party(it->second.getParty());
        response->set_state(it->second.getState());
        response->set_votescount(it->second.getCount());
    }
    return grpc::Status::OK;
}