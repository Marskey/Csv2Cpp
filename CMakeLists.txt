cmake_minimum_required(VERSION 3.0.0)
project(Csv2Cpp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(BUILD_TEST "Build test" off)
include(Csv2Cpp.cmake)

if (BUILD_TEST)
    include(test.cmake)
endif (BUILD_TEST)