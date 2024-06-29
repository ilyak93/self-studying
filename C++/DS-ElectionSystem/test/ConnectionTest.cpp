
#include "ZkConnector.hpp"
#include <iostream>

int main() {
    try {
        ZkConnector connector;
        connector.connect("localhost:2181");
        std::cout << "Successfully connected to ZooKeeper" << std::endl;

        zhandle_t* zk = connector.getZooKeeper();
        // Use zk for ZooKeeper operations...

        connector.close();
        std::cout << "Successfully closed ZooKeeper connection" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}