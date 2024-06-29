#include "ZkConnector.hpp"
#include <stdexcept>
#include <chrono>

ZkConnector::ZkConnector() : zookeeper(nullptr), connected(false) {}

ZkConnector::~ZkConnector() {
    close();
}

void ZkConnector::watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx) {
    ZkConnector* connector = static_cast<ZkConnector*>(watcherCtx);
    if (state == ZOO_CONNECTED_STATE) {
        std::lock_guard<std::mutex> lock(connector->mutex);
        connector->connected = true;
        connector->connectedSignal.notify_all();
    }
}

void ZkConnector::connect(const std::string& host) {
    zookeeper = zookeeper_init(host.c_str(), watcher, 5000, nullptr, this, 0);
    if (!zookeeper) {
        throw std::runtime_error("Failed to initialize ZooKeeper connection");
    }

    std::unique_lock<std::mutex> lock(mutex);
    if (!connectedSignal.wait_for(lock, std::chrono::seconds(10), [this] { return connected; })) {
        throw std::runtime_error("Timeout waiting for ZooKeeper connection");
    }
}

void ZkConnector::close() {
    if (zookeeper) {
        zookeeper_close(zookeeper);
        zookeeper = nullptr;
    }
    connected = false;
}

zhandle_t* ZkConnector::getZooKeeper() {
    if (!zookeeper || zoo_state(zookeeper) != ZOO_CONNECTED_STATE) {
        throw std::runtime_error("ZooKeeper is not connected.");
    }
    return zookeeper;
}