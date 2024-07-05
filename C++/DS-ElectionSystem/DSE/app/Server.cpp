#include "Server.hpp"
#include <iostream>

#include "Server.hpp"
#include <iostream>

// Initialize static members
std::unique_ptr<GreetingServer> Server::grpcServer;
std::unique_ptr<GreetingClient> Server::grpcClient;
std::unique_ptr<GreetingPaxosServer> Server::grpcPaxosServer;
std::unique_ptr<ZkManager> Server::zkManager;
int Server::serverId = 0;
std::atomic<int> Server::votesCounter(0);
std::map<int, std::future<Vote>> Server::votesInDistributionProcess;
std::atomic<bool> Server::electionsStarted(false);
std::atomic<bool> Server::electionsEnded(false);
std::atomic<bool> Server::finishedAll(false);
std::atomic<int> Server::sendingRemoteVoteCounter(0);
std::atomic<int> Server::sendingRemoteVoteMutex(-1);
std::atomic<bool> Server::receiveNewVotes(true);
std::string Server::state;
std::map<std::string, int> Server::stateToElectors;
std::map<int, std::string> Server::clientIdToOriginState;
std::atomic<bool> Server::choseWinner(false);
std::string Server::winner;
std::set<std::string> Server::allStates;

void Server::initialize(const std::vector<std::string>& argv) {
    electionsStarted = false;
    std::string restServerPort = argv[0];
    int grpcServerPort = std::stoi(argv[1]);
    int grpcPaxosServerPort = std::stoi(argv[2]);
    state = argv[3];
    serverId = std::stoi(argv[4]);
    stateToElectors = ReadStates(argv[5]).getStatesMap();
    clientIdToOriginState = ReadVoters(argv[6]).getVotersMap();

    std::string localhost = "localhost";

    // Create the ZooKeeper manager
    std::string address = "127.0.0.1";
    int port = 2181;
    zkManager = std::make_unique<ZkManager>(address, port, state, serverId, localhost, grpcPaxosServerPort, grpcServerPort);

    // Initialize gRPC server
    grpcServer = initializeGrpcServer(serverId, grpcServerPort, state);

    // Initialize gRPC Paxos server
    grpcPaxosServer = initializeGreetingPaxosServer(serverId, grpcPaxosServerPort);
}


Server::Server(const std::vector<std::string>& argv) {
    initialize(argv);
}

void Server::run() {
    pollElectionsStart();
    std::cout << "Start to process future votes" << std::endl;
    processFutureVotes();
    std::cout << "Finished to process future votes" << std::endl;
    std::cout << "Start to broadcast" << std::endl;
    broadcastAllVotesInState();
    std::cout << "Finished to broadcast" << std::endl;
    finishedAll = true;
    std::cout << "Getting the winner" << std::endl;
    std::string winner = getWinner();
    std::cout << "The winner is: " << winner << std::endl;
}

std::unique_ptr<GreetingServer> Server::initializeGrpcServer(int id, int port, const std::string& state) {
    auto server = std::make_unique<GreetingServer>(id, port, state);
    std::cout << "Initialized gRPC server." << std::endl;
    return server;
}

std::unique_ptr<GreetingPaxosServer> Server::initializeGreetingPaxosServer(int id, int port) {
    auto server = std::make_unique<GreetingPaxosServer>(id, port);
    std::cout << "Initialized gRPC Paxos server." << std::endl;
    return server;
}

void Server::processFutureVotes() {
    while (!electionsEnded || !zkManager->isAllFinishedRemoteSending() || !votesInDistributionProcess.empty()) {
        std::cout << "Wait for " << votesInDistributionProcess.size() << " futures" << std::endl;
        for (auto it = votesInDistributionProcess.begin(); it != votesInDistributionProcess.end(); ++it) {
            if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                votesInDistributionProcess.erase(it);
                break;
            }
        }
    }
}

void Server::pollElectionsStart() {
    while (!electionsStarted || !zkManager->isStarted()) {
        if (electionsStarted) {
            zkManager->registerStartElections();
        }
    }
    zkManager->printInitAndStartedChildren();
}

void Server::broadcastAllVotesInState() {
    std::vector<std::string> addressesInState;
    try {
        addressesInState = zkManager->getCurrentStateAddressesForBroadcast();
    } catch (const KeeperException& e) {
        // Handle KeeperException
    } catch (const InterruptedException& e) {
        // Handle InterruptedException
    }

    std::vector<std::shared_ptr<Vote>> broadcastVotes = VotesMap::copyAll();
    int grpcId = 0;
    std::map<int, std::unique_ptr<GreetingClient>> grpcClients;

    for (const auto& vote : broadcastVotes) {
        auto grpcClient = std::make_unique<GreetingClient>(addressesInState);
        grpcClient->sendVoteFuture(vote);
        grpcClients[grpcId] = std::move(grpcClient);
        grpcId++;
    }

    while (!grpcClients.empty()) {
        for (auto it = grpcClients.begin(); it != grpcClients.end(); ++it) {
            if (it->second->isAllFutureSendsDone()) {
                grpcClients.erase(it);
                break;
            }
        }
        std::cout << "Still broadcasting" << std::endl;
    }

    bool registeredEndBroadcast = false;
    while (!zkManager->isEndedBroadcasts()) {
        if (!registeredEndBroadcast) {
            zkManager->registerEndBroadcast();
            registeredEndBroadcast = true;
        }
        std::cout << "Wait to end broadcast" << std::endl;
    }
}

std::string Server::getWinner() {
    if (choseWinner) {
        return winner;
    }

    std::map<VotesCountKey, VotesCount> allVotesCountsMap;

    try {
        auto grpcDistribution = std::make_unique<GreetingClient>(zkManager->getAddressInEachState());
        allVotesCountsMap = grpcDistribution->getAllVotesCounts();
    } catch (const InterruptedException& e) {
        std::cout << "Future interrupted in countAllVotes: " << e.what() << std::endl;
    } catch (const KeeperException& e) {
        std::cout << "Zookeeper exception in countAllVotes: " << e.what() << std::endl;
    }

    std::vector<Distribution> distributions = VotesMap::getDistribution(allVotesCountsMap, stateToElectors);
    std::string winnerParty;
    int maxElectors = 0;

    for (const auto& distribution : distributions) {
        if (distribution.getElectors() >= maxElectors) {
            winnerParty = distribution.getParty();
            maxElectors = distribution.getElectors();
        }
    }

    choseWinner = true;
    winner = winnerParty;
    return winnerParty;
}