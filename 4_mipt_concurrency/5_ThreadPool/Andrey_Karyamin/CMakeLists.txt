cmake_minimum_required(VERSION 3.3)
project(Andrey_Karyamin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES main.cpp ThreadPool.h ThreadPool.hpp ConcurrentMergeSort.h)
add_executable(ThreadPool ${SOURCE_FILES})