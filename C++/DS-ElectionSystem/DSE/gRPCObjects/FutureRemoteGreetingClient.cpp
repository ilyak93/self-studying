#include "FutureRemoteGreetingClient.hpp"
#include "Server.hpp"
#include <iostream>

FutureRemoteGreetingClient::~FutureRemoteGreetingClient() {
    if (worker.joinable()) {
        worker.join();
    }
}

std::future<Vote> FutureRemoteGreetingClient::calculate(const Vote& vote) {
    std::future<Vote> future = promise.get_future();

    worker = std::thread([this, vote]() {
        try {
            Server::sendingRemoteVoteCounter++;

            std::string originStateAddress = Server::zkManager->getAddressInAnotherState(vote.getOriginState());
            GreetingClient greetingClient(originStateAddress);
            greetingClient.sendVoteBlocking(vote);

            Server::sendingRemoteVoteCounter--;

            promise.set_value(vote);
        } catch (const std::exception& e) {
            std::cerr << "Caught exception: " << e.what() << std::endl;
            promise.set_exception(std::current_exception());
        }
    });

    return future;
}