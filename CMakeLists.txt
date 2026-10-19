cmake_minimum_required(VERSION 3.16)
project(cpp_matrix CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(matrix_demo src/main.cpp)

enable_testing()
add_executable(test_matrix tests/test_matrix.cpp)
add_test(NAME test_matrix COMMAND test_matrix)
add_test(NAME demo COMMAND matrix_demo)
