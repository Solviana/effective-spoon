cmake_minimum_required(VERSION 3.5)

include_directories(googletest/googletest/include/gtest)
include_directories(googletest/googletest/include)
add_subdirectory(googletest)

add_executable(Test test_input.cpp test_polygeo.cpp main.cpp)
target_link_libraries(Test gtest)
target_include_directories(Test PUBLIC ../src/input PUBLIC ../src/common PUBLIC ../src/poly)
