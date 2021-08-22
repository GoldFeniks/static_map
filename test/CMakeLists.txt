cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(../include)

add_executable(test
        test.cpp
        test_simple.cpp
)

target_link_libraries(test GTest::GTest GTest::Main)

enable_testing()
add_test(test "./test")
