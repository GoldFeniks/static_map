cmake_minimum_required(VERSION 3.21)
project(sample)

set(CMAKE_CXX_STANDARD 20)

include_directories(../include)

file(GLOB SAMPLES "*.cpp")
foreach(sample ${SAMPLES})
    get_filename_component(name ${sample} NAME_WLE)
    add_executable(${name} ${sample})
endforeach(sample)
