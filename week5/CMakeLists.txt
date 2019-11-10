cmake_minimum_required(VERSION 3.7)
project(week5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Nineteen.cpp words1.h words2.h freqs1.h freqs2.h)
add_executable(week5 Nineteen.cpp)