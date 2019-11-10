cmake_minimum_required(VERSION 3.7)
project(week4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES Fourteen.cpp)
add_executable(week4 ${SOURCE_FILES})