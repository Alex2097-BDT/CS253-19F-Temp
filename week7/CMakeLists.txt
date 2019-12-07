cmake_minimum_required(VERSION 3.7)
project(week7)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES TwentySix.cpp TwentySeven.cpp)
add_executable(week7 ${SOURCE_FILES})