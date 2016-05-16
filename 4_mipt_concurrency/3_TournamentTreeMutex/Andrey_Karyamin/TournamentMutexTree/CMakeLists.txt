cmake_minimum_required(VERSION 3.3)
project(TournamentMutexTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES main.cpp tree_mutex.cpp tree_mutex.h)
add_executable(TournamentMutexTree ${SOURCE_FILES})