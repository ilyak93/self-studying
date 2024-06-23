#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <map>
#include <future>
#include <atomic>
#include <memory>
#include<set>
#include <grpcpp/grpcpp.h>
//#include "zk_manager.h"
//#include "greeting_server.h"
//#include "greeting_paxos_server.h"
//#include "greeting_client.h"
//#include "votes_map.h"
//#include "read_states.h"
//#include "read_voters.h"

class Server {
public:
    Server(const std::vector<std::string>& argv);
    void run();

private:
    std::unique_ptr<GreetingServer> initializeGrpcServer(int id, int port, const std::string& state);
    std::unique_ptr<GreetingPaxosServer> initializeGreetingPaxosServer(int id, int port);
    void processFutureVotes();
    void pollElectionsStart();
    void broadcastAllVotesInState();
    std::string getWinner();

    std::unique_ptr<GreetingServer> grpcServer;
    std::unique_ptr<GreetingClient> grpcClient;
    std::unique_ptr<GreetingPaxosServer> grpcPaxosServer;
    std::unique_ptr<ZkManager> zkManager;
    int serverId;
    std::atomic<int> votesCounter{0};
    std::map<int, std::future<Vote>> votesInDistributionProcess;
    std::atomic<bool> electionsStarted{false};
    std::atomic<bool> electionsEnded{false};
    std::atomic<bool> finishedAll{false};
    std::atomic<int> sendingRemoteVoteCounter{0};
    std::atomic<int> sendingRemoteVoteMutex{-1};
    std::atomic<bool> receiveNewVotes{true};
    std::string state;
    std::map<std::string, int> stateToElectors;
    std::map<int, std::string> clientIdToOriginState;
    std::atomic<bool> choseWinner{false};
    std::string winner;
    std::set<std::string> allStates;
};

#endif // SERVER_H