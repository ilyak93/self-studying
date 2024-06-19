cmake_minimum_required(VERSION 3.22)
project (Server)

set(CMAKE_CXX_STANDARD 11)


# Find the gRPC package
find_package(gRPC CONFIG REQUIRED)

add_executable(Server Server.cpp Server.hpp)
target_link_libraries(Server gRPC::grpc++)


