cmake_minimum_required(VERSION 3.10)
project(CPPPractice)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/include"
    "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/generated"
    "C:/Crow/include"
    "C:/asio-1.30.2/include"
    "C:/jsoncpp/include"
    "C:/grpc/include"
)

# Set library directories
link_directories("C:/grpc/lib")

# Set library directories
link_directories("C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/Debug")

# Find packages
find_package(Protobuf CONFIG REQUIRED)

# ZooKeeper
#set(ZOOKEEPER_INCLUDE_DIR "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/include")
#set(ZOOKEEPER_LIBRARIES "C:/ZooKeeperC/apache-zookeeper-3.9.2/zookeeper-client/zookeeper-client-c/Debug/zookeeper.lib")

# Define gRPC libraries
set(GRPC_LIBRARIES
    grpc
    grpc++
    grpc++_alts
    grpc++_error_details
    grpc++_reflection
    grpc++_unsecure
    grpc_unsecure
    gpr
    cares
    re2
    upb_base_lib
    upb_json_lib
    upb_mem_lib
    upb_message_lib
    upb_textformat_lib
    absl_base
    absl_raw_hash_set
    absl_str_format_internal
    absl_strings
    absl_strings_internal
    zlib
    ssl
    crypto
)

# Add subdirectory for DSE
add_subdirectory(DSE)