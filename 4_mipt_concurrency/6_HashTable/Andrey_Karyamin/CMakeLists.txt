cmake_minimum_required(VERSION 3.2)
project(Andrey_Karyamin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES main.cpp ThreadSafeHashSet.h ThreadSafeHashSet.hpp)
add_executable(Andrey_Karyamin ${SOURCE_FILES})