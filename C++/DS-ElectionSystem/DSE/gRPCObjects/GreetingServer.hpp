#pragma once
#include <grpcpp/grpcpp.h>
#include "Vote.grpc.pb.h"
#include <atomic>
#include <mutex>
#include <future>
#include <string>
#include <memory>
#include "ServerInterceptor.hpp"

class GreetingServer final : public protos::Greeter::Service {
private:
    int id;
    std::unique_ptr<grpc::Server> greetingServer;
    std::string state;
    ServerInterceptorFactory interceptorFactory;
    std::mutex sendingRemoteVoteMutex; // Change this from atomic to mutex

public:
    GreetingServer(int id, int port, const std::string& state);
    void shutdown();
    void addRestrict(const std::string& client, const std::string& procedure);
    grpc::Status ReceiveVote(grpc::ServerContext* context, const protos::VoteRequest* request, protos::VoteReply* response) override;
    grpc::Status ReceiveStartElections(grpc::ServerContext* context, const protos::StartElectionsRequest* request, protos::StartElectionsReply* response) override;
    grpc::Status ReceiveEndElections(grpc::ServerContext* context, const protos::EndElectionsRequest* request, protos::EndElectionsReply* response) override;
    grpc::Status ReceiveStatus(grpc::ServerContext* context, const protos::VotesCountForPartyRequest* request, protos::VotesCountForPartyReply* response) override;
    grpc::Status ReciveVotesCount(grpc::ServerContext* context, const protos::VotesCountForPartyRequest* request, grpc::ServerWriter<protos::VotesCountForPartyReply>* writer) override;
};