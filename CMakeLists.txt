cmake_minimum_required(VERSION 3.0.0)
project(Csv2Cpp VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(Csv2Cpp.cmake)

option(BUILD_TEST "Build test" ON)
set(BUILD_GMOCK off)
set(INSTALL_GTEST off)
set(gtest_force_shared_crt on)
include(test.cmake)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})