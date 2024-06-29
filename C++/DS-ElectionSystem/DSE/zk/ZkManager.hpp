#ifndef ZK_MANAGER_HPP
#define ZK_MANAGER_HPP

#include "ZkSync.hpp"
#include <vector>
#include <string>
#include <map>

class ZkManager : public ZkSync {
private:
    int serverIndex;
    std::string state;
    std::string nodePath;
    const std::string initPath = "/init";
    const std::string electionsStartPath = "/electionsStart";
    const std::string finishedRemoteSending = "/finishedRemoteSending";
    const std::string endBroadcast = "/endBroadcast";
    const std::string timePath = "/time";

    void createNode(const std::string& path, bool isPersistent = true);
    void createServerNode(const std::string& path, const std::string& data);
    std::vector<std::string> getChildren(const std::string& path);
    std::string getData(const std::string& path);

public:
    ZkManager(const std::string& address, int port, const std::string& state, int serverIndex, 
              const std::string& localhost, int grpcPaxosServerPort, int grpcServerPort);

    std::vector<std::string> getLiveServers();
    std::vector<std::string> getCurrentStateAddressesForPaxos();
    std::vector<std::string> getCurrentStateAddressesForBroadcast();
    int getServerIndex() const;
    std::string getState() const;
    std::string getAddressInAnotherState(const std::string& anotherState);
    std::vector<std::string> getAddressInEachState();
    void registerStartElections();
    bool isStarted();
    void printInitAndStartedChildren();
    std::vector<std::string> getAddressesToStartElections();
    void registerFinishedRemoteSending();
    bool isAllFinishedRemoteSending();
    void registerEndBroadcast();
    bool isEndedBroadcasts();
    int64_t getTime();
};

#endif // ZK_MANAGER_HPP