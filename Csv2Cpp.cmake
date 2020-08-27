set(SRC_FILE 
    csv.hpp
    main.cpp
)

include_directories(thirdparty/fmt/include)
add_subdirectory(thirdparty/fmt)

add_executable(Csv2Cpp ${SRC_FILE})
target_link_libraries(Csv2Cpp fmt::fmt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
