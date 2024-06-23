# Define the DSE library
add_library(DSE
    app/Server.cpp
    app/controllers/VoteController.cpp
    app/controllers/VotesMap.cpp
    app/models/CommandElections.cpp
    app/models/ControllerMessage.cpp
    app/models/Distribution.cpp
    app/models/Vote.cpp
    app/models/VotesCount.cpp
    app/models/VotesCountKey.cpp
    app/utils/ReadStates.cpp
    app/utils/ReadVoters.cpp
    controllers/VoteController.cpp
    controllers/VotesMap.cpp
    gRPCObjects/interceptors/ClientInterceptor.cpp
    gRPCObjects/interceptors/ServerInterceptor.cpp
    models/ControllerMessage.cpp
    models/Vote.cpp
    protos/Paxos.grpc.pb.cc
    protos/Paxos.pb.cc
    protos/Vote.grpc.pb.cc
    protos/Vote.pb.cc
    zk/ZkConnector.cpp
    zk/ZkManager.cpp
    zk/ZkSync.cpp
)

# Include directories
target_include_directories(DSE PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/gRPCObjects
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/protos
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/zk
    ${ZOOKEEPER_INCLUDE_DIR}
    "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/include"
    "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/generated"
    "C:/grpc/include"
)

# Link libraries
target_link_libraries(DSE PUBLIC 
    "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/Debug/zookeeper.lib"
    ${GRPC_LIBRARIES}
    "C:/grpc/lib/libprotobuf.lib"
)

# Add executable
add_executable(DSEApp app/main.cpp)
target_link_libraries(DSEApp PRIVATE DSE)