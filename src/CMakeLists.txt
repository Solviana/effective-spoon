add_executable(Solution main.c)

add_subdirectory(poly)
add_subdirectory(input)
add_subdirectory(game)

target_link_libraries(Solution Game Input Poly)
target_include_directories(Solution PUBLIC game PUBLIC input PUBLIC game)