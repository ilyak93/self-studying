#ifndef ZK_MANAGER_HPP
#define ZK_MANAGER_HPP

#include "ZkSync.hpp"
#include <string>
#include <vector>
#include <map>

class ZkManager : public ZkSync {
public:
    ZkManager(const std::string& address, int port, const std::string& state, int serverIndex,
              const std::string& localhost, int grpcPaxosServerPort, int grpcServerPort);
    std::vector<std::string> getLiveServers() const;
    std::vector<std::string> getCurrentStateAddressesForPaxos() const;
    std::vector<std::string> getCurrentStateAddressesForBroadcast() const;
    int getServerIndex() const;
    std::string getState() const;
    std::string getAddressInAnotherState(const std::string& anotherState) const;
    std::vector<std::string> getAddressInEachState() const;
    void registerStartElections();
    bool isStarted() const;
    void printInitAndStartedChildren() const;
    std::vector<std::string> getAddressesToStartElections() const;
    void registerFinishedRemoteSending();
    bool isAllFinishedRemoteSending() const;
    void registerEndBroadcast();
    bool isEndedBroadcasts() const;
    long getTime() const;

private:
    int serverIndex;
    std::string state;
    std::string nodePath;
    std::string initPath;
    std::string electionsStartPath;
    std::string finishedRemoteSending;
    std::string endBroadcast;
    std::string timePath;
};

#endif // ZK_MANAGER_HPP