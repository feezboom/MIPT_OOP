cmake_minimum_required(VERSION 3.3)
project(Peterson_mutex)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp Mutex.h)
add_executable(Peterson_mutex ${SOURCE_FILES})