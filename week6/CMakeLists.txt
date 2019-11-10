cmake_minimum_required(VERSION 3.7)
project(week6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES ThirtyOne.cpp)
add_executable(week6 ThirtyOne.cpp)

# find_package(Threads REQUIRED)
# target_link_libraries(week6 Threads::Threads)