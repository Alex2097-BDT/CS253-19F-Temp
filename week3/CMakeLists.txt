cmake_minimum_required(VERSION 3.7)
project(week3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES Nine.cpp)
add_executable(week3 ${SOURCE_FILES})