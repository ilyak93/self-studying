#include "ZkManager.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <zookeeper.h>


ZkManager::ZkManager(const std::string& address, int port, const std::string& state, int serverIndex,
                     const std::string& localhost, int grpcPaxosServerPort, int grpcServerPort)
    : ZkSync(address), serverIndex(serverIndex), state(state) {
    
    root = "/" + state + "/servers";
    
    if (zk != nullptr) {
        try {
            createNode(root);
            std::string data = localhost + ":" + std::to_string(grpcPaxosServerPort);
            createServerNode(root + "/" + std::to_string(serverIndex), data);

            createNode(initPath);
            data = localhost + ":" + std::to_string(grpcServerPort);
            createServerNode(initPath + "/" + state + std::to_string(serverIndex), data);

            createNode(electionsStartPath);
            createNode(endBroadcast);
            createNode(finishedRemoteSending);
            createNode(timePath);
        } catch (const std::exception& e) {
            std::cerr << "Error in ZkManager initialization: " << e.what() << std::endl;
            throw;
        }
    }
}

void ZkManager::createNode(const std::string& path, bool isPersistent) {
    std::string currentPath;
    std::istringstream iss(path);
    std::string token;

    while (std::getline(iss, token, '/')) {
        if (!token.empty()) {
            currentPath += "/" + token;
            struct Stat stat;
            int rc = zoo_exists(zk, currentPath.c_str(), 0, &stat);
            if (rc == ZNONODE) {
                rc = zoo_create(zk, currentPath.c_str(), nullptr, 0, &ZOO_OPEN_ACL_UNSAFE, 
                                isPersistent ? 0 : ZOO_EPHEMERAL, nullptr, 0);
                if (rc != ZOK) {
                    throw std::runtime_error("Failed to create node: " + currentPath);
                }
                std::cout << "Created node " << currentPath << std::endl;
            }
        }
    }
}

void ZkManager::createServerNode(const std::string& path, const std::string& data) {
    int rc = zoo_create(zk, path.c_str(), data.c_str(), data.length(), &ZOO_OPEN_ACL_UNSAFE, 
                        ZOO_EPHEMERAL, nullptr, 0);
    if (rc != ZOK) {
        throw std::runtime_error("Failed to create server node: " + path);
    }
    std::cout << "Created node " << path << std::endl;
}

std::vector<std::string> ZkManager::getChildren(const std::string& path) {
    std::vector<std::string> children;
    struct String_vector sv;
    int rc = zoo_get_children(zk, path.c_str(), 0, &sv);
    if (rc == ZOK) {
        for (int i = 0; i < sv.count; ++i) {
            children.push_back(sv.data[i]);
        }
        deallocate_String_vector(&sv);
    } else {
        throw std::runtime_error("Failed to get children for path: " + path);
    }
    return children;
}

std::string ZkManager::getData(const std::string& path) {
    char buffer[1024];
    int buffer_len = sizeof(buffer);
    struct Stat stat;
    int rc = zoo_get(zk, path.c_str(), 0, buffer, &buffer_len, &stat);
    if (rc == ZOK) {
        return std::string(buffer, buffer_len);
    } else {
        throw std::runtime_error("Failed to get data for path: " + path);
    }
}

std::vector<std::string> ZkManager::getLiveServers() {
    return getChildren(root);
}

std::vector<std::string> ZkManager::getCurrentStateAddressesForPaxos() {
    std::vector<std::string> liveServers = getLiveServers();
    std::vector<std::string> addresses;
    for (const auto& server : liveServers) {
        addresses.push_back(getData(root + "/" + server));
    }
    return addresses;
}

std::vector<std::string> ZkManager::getCurrentStateAddressesForBroadcast() {
    std::vector<std::string> addresses;
    std::vector<std::string> servers = getChildren(initPath);
    for (const auto& server : servers) {
        if (server.find(state) != std::string::npos) {
            addresses.push_back(getData(initPath + "/" + server));
        }
    }
    return addresses;
}

int ZkManager::getServerIndex() const {
    return serverIndex;
}

std::string ZkManager::getState() const {
    return state;
}

std::string ZkManager::getAddressInAnotherState(const std::string& anotherState) {
    std::vector<std::string> servers = getChildren(initPath);
    for (const auto& server : servers) {
        if (server.find(anotherState) != std::string::npos) {
            return getData(initPath + "/" + server);
        }
    }
    return "";
}

std::vector<std::string> ZkManager::getAddressInEachState() {
    std::map<std::string, std::string> addressesInStates;
    std::vector<std::string> servers = getChildren(initPath);
    for (const auto& server : servers) {
        std::string stateOfServer = server;
        stateOfServer.erase(std::remove_if(stateOfServer.begin(), stateOfServer.end(), ::isdigit), stateOfServer.end());
        if (addressesInStates.find(stateOfServer) == addressesInStates.end()) {
            addressesInStates[stateOfServer] = getData(initPath + "/" + server);
        }
    }
    std::vector<std::string> addresses;
    for (const auto& pair : addressesInStates) {
        addresses.push_back(pair.second);
    }
    return addresses;
}

void ZkManager::registerStartElections() {
    createNode(electionsStartPath + "/" + state + std::to_string(serverIndex), false);
}

bool ZkManager::isStarted() {
    std::vector<std::string> startedServers = getChildren(electionsStartPath);
    std::vector<std::string> upServers = getChildren(initPath);
    return startedServers.size() == upServers.size();
}

void ZkManager::printInitAndStartedChildren() {
    std::vector<std::string> startedServers = getChildren(electionsStartPath);
    std::vector<std::string> upServers = getChildren(initPath);
    std::cout << "electionsStartPath children " << startedServers.size() << std::endl;
    std::cout << "initPath children " << upServers.size() << std::endl;
}

std::vector<std::string> ZkManager::getAddressesToStartElections() {
    std::vector<std::string> addresses;
    std::vector<std::string> servers = getChildren(initPath);
    for (const auto& server : servers) {
        addresses.push_back(getData(initPath + "/" + server));
    }
    return addresses;
}

void ZkManager::registerFinishedRemoteSending() {
    createNode(finishedRemoteSending + "/" + state + std::to_string(serverIndex), false);
}

bool ZkManager::isAllFinishedRemoteSending() {
    std::vector<std::string> finishedServers = getChildren(finishedRemoteSending);
    std::vector<std::string> upServers = getChildren(initPath);
    return finishedServers.size() == upServers.size();
}

void ZkManager::registerEndBroadcast() {
    createNode(endBroadcast + "/" + state + std::to_string(serverIndex), false);
}

bool ZkManager::isEndedBroadcasts() {
    std::vector<std::string> endedBroadcasts = getChildren(endBroadcast);
    std::vector<std::string> upServers = getChildren(initPath);
    return endedBroadcasts.size() == upServers.size();
}

int64_t ZkManager::getTime() {
    struct Stat stat;
    int rc = zoo_exists(zk, timePath.c_str(), 0, &stat);
    if (rc == ZOK) {
        return stat.ctime;
    } else {
        throw std::runtime_error("Failed to get time");
    }
}