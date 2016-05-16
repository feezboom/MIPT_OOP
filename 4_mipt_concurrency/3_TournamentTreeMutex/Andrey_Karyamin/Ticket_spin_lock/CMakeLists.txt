cmake_minimum_required(VERSION 3.3)
project(Ticket_spin_lock)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp TicketSpinLock.hpp)
add_executable(Ticket_spin_lock ${SOURCE_FILES})