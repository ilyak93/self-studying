// FuturePaxosGreetingClient.cpp

#include "FuturePaxosGreetingClient.hpp"
#include <thread>

std::future<Vote> FuturePaxosGreetingClient::calculate(const std::vector<std::string>& addresses, int serverId, const Vote& vote) {
    return std::async(std::launch::async, [addresses, serverId, vote]() {
        GreetingPaxosClient greetingPaxosClient(addresses, serverId);
        Vote updatedVote = greetingPaxosClient.sendVote(vote);
        // greetingPaxosClient will be automatically destroyed when it goes out of scope
        return updatedVote;
    });
}