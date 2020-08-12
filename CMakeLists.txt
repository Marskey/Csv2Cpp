cmake_minimum_required(VERSION 3.0.0)
project(Csv2Cpp VERSION 0.1.0)

aux_source_directory(. DIR_SRCS)

add_subdirectory(thirdparty/fmt)

add_executable(Csv2Cpp ${DIR_SRCS})
target_link_libraries(Csv2Cpp fmt::fmt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})