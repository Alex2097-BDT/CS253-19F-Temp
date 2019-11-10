cmake_minimum_required(VERSION 3.7)
project(week2)

# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES Five.cpp)
add_executable(week2 ${SOURCE_FILES})