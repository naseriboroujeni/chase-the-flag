cmake_minimum_required(VERSION 3.10)
project(MyWebSocketProject)

set(CMAKE_CXX_STANDARD 17)

# Find WebSocket++ package
find_package(websocketpp REQUIRED)

# Server
add_subdirectory(server)

# Client
add_subdirectory(client)
