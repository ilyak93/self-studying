// FuturePaxosGreetingClient.hpp

#ifndef FUTURE_PAXOS_GREETING_CLIENT_HPP
#define FUTURE_PAXOS_GREETING_CLIENT_HPP

#include <future>
#include <vector>
#include <string>
#include "Vote.hpp"
#include "GreetingPaxosClient.hpp"

class FuturePaxosGreetingClient {
private:
    std::future<Vote> future;

public:
    FuturePaxosGreetingClient() = default;
    ~FuturePaxosGreetingClient() = default;

    std::future<Vote> calculate(const std::vector<std::string>& addresses, int serverId, const Vote& vote);
};

#endif // FUTURE_PAXOS_GREETING_CLIENT_HPP

