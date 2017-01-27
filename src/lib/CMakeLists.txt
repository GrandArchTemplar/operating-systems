cmake_minimum_required(VERSION 3.0.0)

project(lib)

add_library(graph graph.c)
add_library(copy copy.c)
add_library(matrix matrix.c)
add_library(concurrent_matrix concurrent_matrix.c)

target_link_libraries(concurrent_matrix matrix)
