cmake_minimum_required(VERSION 3.5)
project(thread_safe_queue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES main.cpp thread_safe_queue.h)
add_executable(thread_safe_queue ${SOURCE_FILES})