cmake_minimum_required(VERSION 3.21)
project(sample)

set(CMAKE_CXX_STANDARD 20)

include_directories(../include)

add_executable(sample ../include/feniks/static_map.hpp sample.cpp)
