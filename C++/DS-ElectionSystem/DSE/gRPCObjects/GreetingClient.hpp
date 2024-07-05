// GreetingClient.hpp
#ifndef GREETING_CLIENT_HPP
#define GREETING_CLIENT_HPP

#include <grpcpp/grpcpp.h>
#include "Vote.grpc.pb.h"
#include "Vote.hpp"
#include "VotesCount.hpp"
#include "VotesCountKey.hpp"
#include "OneTimeUseElement.hpp"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <future>

class GreetingClient {
private:
    std::shared_ptr<grpc::Channel> singleChannel;
    std::vector<std::shared_ptr<grpc::Channel>> channels;
    static std::unordered_map<std::string, std::shared_ptr<grpc::Channel>> remoteChannels;
    std::vector<std::future<protos::VoteReply>> sendsInProcess;

    std::unique_ptr<protos::Greeter::Stub> createStub(const std::shared_ptr<grpc::Channel>& channel);
    OneTimeUseElement<std::future<protos::StartElectionsReply>> sendStartElections(const protos::StartElectionsRequest& request, const std::shared_ptr<grpc::Channel>& channel);
    OneTimeUseElement<std::future<protos::EndElectionsReply>> sendEndElections(const protos::EndElectionsRequest& request, const std::shared_ptr<grpc::Channel>& channel);

public:
    explicit GreetingClient(const std::string& address);
    explicit GreetingClient(const std::vector<std::string>& addresses);
    ~GreetingClient();

    void shutdown();
    bool isAllFutureSendsDone();
    std::vector<std::future<protos::VoteReply>> sendVoteFuture(const Vote& vote);
    void sendVoteBlocking(const Vote& vote);
    void startElections();
    void endElections();
    std::unordered_map<VotesCountKey, VotesCount> getAllVotesCounts();
    VotesCount getStatus(const std::string& party, const std::string& state);
};

#endif // GREETING_CLIENT_HPP