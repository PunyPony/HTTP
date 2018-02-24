#check version

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -std=c++17 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-02")
set(CMAKE_BUILD_TYPE Debug)

include_directories(src/)

set(SRC
    src/main.cc
    src/ServersHandler.cc
    src/HTTPServerOptions.cc
    src/HTTPServer.cc
    src/ResponseBuilder.cc
    src/ServerConfig.cc
)

add_executable(myhttpd ${SRC})
target_link_libraries(myhttpd pthread)
