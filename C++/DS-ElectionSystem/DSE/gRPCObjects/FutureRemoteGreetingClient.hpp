#ifndef FUTURE_REMOTE_GREETING_CLIENT_HPP
#define FUTURE_REMOTE_GREETING_CLIENT_HPP

#include <future>
#include <thread>
#include "Vote.hpp"
#include "GreetingClient.hpp"
#include "ZkManager.hpp"

class FutureRemoteGreetingClient {
private:
    std::thread worker;
    std::promise<Vote> promise;

public:
    FutureRemoteGreetingClient() = default;
    ~FutureRemoteGreetingClient();

    std::future<Vote> calculate(const Vote& vote);
};

#endif // FUTURE_REMOTE_GREETING_CLIENT_HPP