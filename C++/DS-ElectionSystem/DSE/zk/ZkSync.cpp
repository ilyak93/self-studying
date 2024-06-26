#include "ZkSync.hpp"
#include <iostream>
#include <stdexcept>

ZkSync::ZkSync(const std::string& address) {
    if (zk == nullptr) {
        try {
            std::cout << "Starting ZK:" << std::endl;
            zk = zookeeper_init(address.c_str(), watcher, 3000, nullptr, this, 0);
            if (zk == nullptr) {
                throw std::runtime_error("Failed to initialize ZooKeeper");
            }
            std::cout << "Finished starting ZK: " << zk << std::endl;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            zk = nullptr;
        }
        throw ;
    }
}

ZkSync::~ZkSync() {
    if (zk != nullptr) {
        zookeeper_close(zk);
    }
}

void ZkSync::watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx) {
    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            std::cout << "Connected to ZooKeeper" << std::endl;
        } else if (state == ZOO_EXPIRED_SESSION_STATE) {
            std::cout << "ZooKeeper session expired" << std::endl;
        }
    } else if (type == ZOO_CREATED_EVENT) {
            std::cout << "Node created: " << path << std::endl;
    } else if (type == ZOO_DELETED_EVENT) {
        std::cout << "Node deleted: " << path << std::endl;
    } else if (type == ZOO_CHANGED_EVENT) {
            std::cout << "Node changed: " << path << std::endl;
    } else if (type == ZOO_CHILD_EVENT) {
            std::cout << "Child changed: " << path << std::endl;
    }

    ZkSync* zkSync = static_cast<ZkSync*>(watcherCtx);
    std::unique_lock<std::mutex> lock(zkSync->mutex);
    zkSync->cv.notify_one();
}

void ZkSync::wait() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
}