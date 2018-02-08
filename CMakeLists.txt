#check version

cmake_minimum_required(VERSION 2.6)


set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -pthread -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-02")
set(CMAKE_BUILD_TYPE Debug)

include_directories(src/)
set(SRC
    src/header.hh
    src/threads.cc
    src/basiccon.cc
)


add_executable(myhttpd ${SRC})
