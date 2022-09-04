cmake_minimum_required(VERSION 3.5)


add_subdirectory(googletest)

add_executable(Test test_input.cpp test_polygeo.cpp main.cpp)
target_include_directories(Test PUBLIC googletest/googletest/include/gtest)
target_include_directories(Test PUBLIC googletest/googletest/include)
target_link_libraries(Test Poly gtest)
target_include_directories(Test PUBLIC ../src/input PUBLIC ../src/common PUBLIC ../src/poly)
