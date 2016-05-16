cmake_minimum_required(VERSION 3.5)
project(RingBuffer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp spsc_ring_buffer.h)
add_executable(RingBuffer ${SOURCE_FILES})