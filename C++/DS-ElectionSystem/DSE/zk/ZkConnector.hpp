#ifndef ZKCONNECTOR_HPP
#define ZKCONNECTOR_HPP

#include <zookeeper.h>
#include <string>
#include <mutex>
#include <condition_variable>

class ZkConnector {
private:
    zhandle_t* zookeeper;
    std::mutex connectedMutex;
    std::condition_variable connectedCondition;
    bool connected;

    static void watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx);

public:
    ZkConnector();
    ~ZkConnector();

    void connect(const std::string& host);
    void close();
    zhandle_t* getZooKeeper();
};

#endif // ZKCONNECTOR_HPP