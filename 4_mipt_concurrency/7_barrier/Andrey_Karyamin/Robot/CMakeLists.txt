cmake_minimum_required(VERSION 3.5)
project(Robot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp Semaphore.cpp Semaphore.h)
add_executable(Robot ${SOURCE_FILES})