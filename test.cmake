include_directories(thirdparty/googletest/googletest/include)
include_directories(testdata)

set(TEST_SRC_FILE 
    "test.cc"
    "CsvConfigInsts.cpp"
    "CsvConfigInsts.h"
)

add_subdirectory(thirdparty/googletest)

add_executable(test ${TEST_SRC_FILE})
target_link_libraries(test gtest)
