cmake_minimum_required(VERSION 4.0)
project(TrafficSimulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(TrafficSimulator main.cpp
        vehicles.h
        vehicles.cpp
        traffic.h
        traffic.cpp)
