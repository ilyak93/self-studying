#ifndef GREETING_PAXOS_CLIENT_HPP
#define GREETING_PAXOS_CLIENT_HPP

#include <grpcpp/grpcpp.h>
#include "Paxos.grpc.pb.h"
#include "Vote.hpp"
#include "SessionsMap.hpp"
#include "gRPCObjects/interceptors/ClientInterceptor.hpp"
#include "OneTimeUseElement.hpp"
#include <vector>
#include <memory>
#include <future>


class GreetingPaxosClient {
private:
    int serverId;
    std::vector<std::unique_ptr<grpc::Channel>> channels;
    std::vector<std::unique_ptr<protos::PaxosGreeter::Stub>> stubs;

    OneTimeUseElement<std::future<protos::Init>> sendInit(const protos::Init& init, protos::PaxosGreeter::Stub* stub);
    OneTimeUseElement<std::future<protos::Promise>> sendPrepare(const protos::Prepare& prepare, protos::PaxosGreeter::Stub* stub);
    OneTimeUseElement<std::future<protos::Accepted>> sendAccept(const protos::Accept& accept, protos::PaxosGreeter::Stub* stub);
    OneTimeUseElement<std::future<protos::Finish>> sendCommit(const protos::Commit& commit, protos::PaxosGreeter::Stub* stub);

public:
    GreetingPaxosClient(const std::vector<std::string>& addresses, int serverId);
    void shutdown();
    Vote sendVote(const Vote& vote);
};

#endif // GREETING_PAXOS_CLIENT_HPP