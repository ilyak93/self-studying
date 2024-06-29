// GreetingPaxosServer.hpp

#ifndef GREETING_PAXOS_SERVER_HPP
#define GREETING_PAXOS_SERVER_HPP

#include <grpcpp/grpcpp.h>
#include "Paxos.grpc.pb.h"
#include "Vote.hpp"
#include "SessionsMap.hpp"
#include "VotesMap.hpp"
#include <atomic>

class GreetingPaxosServer final : public protos::PaxosGreeter::Service {
private:
    int id;
    std::unique_ptr<grpc::Server> greetingServer;
    static std::atomic<int> sessionsCounter;

public:
    GreetingPaxosServer(int id, int port);
    void shutdown();

    grpc::Status UponReceivingInit(grpc::ServerContext* context, const protos::Init* init, protos::Init* response) override;
    grpc::Status UponReceivingPrepare(grpc::ServerContext* context, const protos::Prepare* prepare, protos::Promise* response) override;
    grpc::Status UponReceivingAccept(grpc::ServerContext* context, const protos::Accept* accept, protos::Accepted* response) override;
    grpc::Status UponReceivingCommit(grpc::ServerContext* context, const protos::Commit* commit, protos::Finish* response) override;
};

#endif // GREETING_PAXOS_SERVER_HPP