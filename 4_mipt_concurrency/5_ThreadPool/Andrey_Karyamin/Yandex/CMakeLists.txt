cmake_minimum_required(VERSION 3.5)
project(thread_pool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES main.cpp thread_pool.h)
add_executable(thread_pool ${SOURCE_FILES})