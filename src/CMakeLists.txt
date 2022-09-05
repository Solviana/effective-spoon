add_executable(Solution main.c)

add_subdirectory(poly)
add_subdirectory(input)
add_subdirectory(game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu11")

target_link_libraries(Solution Game Input Poly m)
target_include_directories(Solution PUBLIC game PUBLIC input PUBLIC poly)