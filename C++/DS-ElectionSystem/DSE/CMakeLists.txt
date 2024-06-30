cmake_minimum_required(VERSION 3.12)
project(DSE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the path to ASIO
set(ASIO_INCLUDE_DIR "C:/asio-1.30.2/include")

# Include ASIO
include_directories(${ASIO_INCLUDE_DIR})

# Add definitions for standalone ASIO and Crow
add_definitions(-DASIO_STANDALONE)
add_definitions(-DCROW_DISABLE_STATIC_DIR)
add_definitions(-DCROW_DISABLE_COMPRESSION)
add_definitions(-DCROW_USE_ASIO)

# If you're on Windows, you might need this
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

# Set ASIO include directory for Crow
set(CROW_ASIO_INCLUDE_DIR ${ASIO_INCLUDE_DIR})

# Find Crow
find_package(Crow CONFIG REQUIRED)

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
    app/paxos/AcceptAndAccepted.cpp
    app/paxos/PrepareAndPromise.cpp
    app/paxos/SessionsMap.cpp
    app/utils/ReadStates.cpp
    app/utils/ReadVoters.cpp
    gRPCObjects/interceptors/ClientInterceptor.cpp
    gRPCObjects/interceptors/ServerInterceptor.cpp
    gRPCObjects/paxos/FuturePaxosGreetingClient.cpp
    gRPCObjects/paxos/GreetingPaxosClient.cpp
    gRPCObjects/paxos/GreetingPaxosServer.cpp
    gRPCObjects/GreetingClient.cpp
    gRPCObjects/GreetingServer.cpp
    gRPCObjects/FutureRemoteGreetingClient.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/gRPCObjects
    ${CMAKE_CURRENT_SOURCE_DIR}/protos
    ${CMAKE_CURRENT_SOURCE_DIR}/app/models
    ${CMAKE_CURRENT_SOURCE_DIR}/app/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/app/paxos
    ${CMAKE_CURRENT_SOURCE_DIR}/app/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/protos
    ${CMAKE_CURRENT_SOURCE_DIR}/zk
    ${ZOOKEEPER_INCLUDE_DIR}
    "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/include"
    "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/generated"
    "C:/grpc/include"
    ${ASIO_INCLUDE_DIR}
)

# Option for threaded/non-threaded version
option(USE_THREADED "Use threaded version of ZooKeeper" ON)

# Set compile definitions based on USE_THREADED option
if(USE_THREADED)
    target_compile_definitions(DSE PUBLIC THREADED)
endif()

# Set ZooKeeper library path
set(ZOOKEEPER_LIB "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/Debug/zookeeper.lib")


# Link libraries
target_link_libraries(DSE PUBLIC
    "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/Debug/zookeeper.lib"
    ${GRPC_LIBRARIES}
    "C:/grpc/lib/libprotobuf.lib"
    Crow::Crow
)

target_compile_definitions(DSE PUBLIC
    ASIO_STANDALONE
    CROW_USE_ASIO
)
# Add executable
add_executable(DSEApp app/main.cpp)
target_link_libraries(DSEApp PRIVATE DSE)