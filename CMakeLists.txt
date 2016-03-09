cmake_minimum_required(VERSION 3.3)
project(juzhen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp lianbiao2.cpp)
add_executable(juzhen ${SOURCE_FILES})