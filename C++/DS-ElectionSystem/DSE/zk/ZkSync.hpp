#ifndef ZK_SYNC_HPP
#define ZK_SYNC_HPP

#include <zookeeper.h>
#include <string>
#include <mutex>
#include <condition_variable>

class ZkSync {
protected:
    std::mutex mutex;
    std::condition_variable cv;
    zhandle_t* zk;
    std::string root;

public:
    ZkSync(const std::string& address);
    virtual ~ZkSync();

    static void watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx);

    // Disable copy constructor and assignment operator
    ZkSync(const ZkSync&) = delete;
    ZkSync& operator=(const ZkSync&) = delete; 

    void wait(); 
};

#endif // ZK_SYNC_HPP