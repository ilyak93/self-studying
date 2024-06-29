#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <map>
#include <future>
#include <atomic>
#include <memory>
#include <set>
#include <grpcpp/grpcpp.h>
#include "zk/ZkManager.hpp"
#include "controllers/VotesMap.hpp"
#include "Vote.hpp"
#include "utils/ReadStates.hpp"
#include "utils/ReadVoters.hpp"
#include <exception>
#include <string>

class GreetingServer;
class GreetingClient;
class GreetingPaxosServer;

class KeeperException : public std::exception {
private:
    std::string message;

public:
    explicit KeeperException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InterruptedException : public std::exception {
private:
    std::string message;

public:
    explicit InterruptedException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class Server {
private:    
    static void initialize(const std::vector<std::string>& argv);
public:
    static std::unique_ptr<GreetingServer> grpcServer;
    static std::unique_ptr<GreetingClient> grpcClient;
    static std::unique_ptr<GreetingPaxosServer> grpcPaxosServer;
    static std::unique_ptr<ZkManager> zkManager;
    static int serverId;
    static std::atomic<int> votesCounter;
    static std::map<int, std::future<Vote>> votesInDistributionProcess;
    static std::atomic<bool> electionsStarted;
    static std::atomic<bool> electionsEnded;
    static std::atomic<bool> finishedAll;
    static std::atomic<int> sendingRemoteVoteCounter;
    static std::atomic<int> sendingRemoteVoteMutex;
    static std::atomic<bool> receiveNewVotes;
    static std::string state;
    static std::map<std::string, int> stateToElectors;
    static std::map<int, std::string> clientIdToOriginState;
    static std::atomic<bool> choseWinner;
    static std::string winner;
    static std::set<std::string> allStates;

    Server::Server(const std::vector<std::string>& argv);
    static void run();

    static std::unique_ptr<GreetingServer> initializeGrpcServer(int id, int port, const std::string& state);
    static std::unique_ptr<GreetingPaxosServer> initializeGreetingPaxosServer(int id, int port);
    static void processFutureVotes();
    static void pollElectionsStart();
    static void broadcastAllVotesInState();
    static std::string getWinner();
};

#endif // SERVER_H