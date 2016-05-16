cmake_minimum_required(VERSION 3.3)
project(BlockingQueue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp BlockingQueue.h BlockingQueue.hpp)
add_executable(BlockingQueue ${SOURCE_FILES})