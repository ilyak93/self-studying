#ifndef ZK_CONNECTOR_HPP
#define ZK_CONNECTOR_HPP

#include <zookeeper.h>
#include <string>
#include <condition_variable>
#include <mutex>

class ZkConnector {
private:
    zhandle_t* zookeeper;
    std::condition_variable connectedSignal;
    std::mutex mutex;
    bool connected;

    static void watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx);

public:
    ZkConnector();
    ~ZkConnector();

    void connect(const std::string& host);
    void close();
    zhandle_t* getZooKeeper();
};

#endif // ZK_CONNECTOR_HPP