include_directories(thirdparty/googletest/googletest/include)

set(TEST_SRC_FILE 
    "test.cc"
)

add_subdirectory(thirdparty/googletest)

add_executable(test ${TEST_SRC_FILE})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
