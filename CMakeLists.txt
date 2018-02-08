#check version

cmake_minimum_required(VERSION 2.6)

set(CMAKE_C_COMPILER "g++-7")
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic -std=c++11")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-02")
set(CMAKE_BUILD_TYPE Debug)

include_directories(src/)

set(SRC
    src/main.cc
    src/HTTPServer.cc)

add_executable(myhttpd ${SRC})
