cmake_minimum_required(VERSION 3.4)
project(CyclicBarrier)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES main.cpp CyclicBarrier.cpp CyclicBarrier.h)
add_executable(CyclicBarrier ${SOURCE_FILES})