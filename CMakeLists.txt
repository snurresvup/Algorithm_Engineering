cmake_minimum_required(VERSION 3.3)
project(Algorithm_Engineering)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(SOURCE_FILES branch_test.cpp)
add_executable(Algorithm_Engineering ${SOURCE_FILES})