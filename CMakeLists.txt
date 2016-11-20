cmake_minimum_required(VERSION 3.0.0)
project(operating-systems VERSION 0.0.0)


add_executable(cp src/task2/main.c)
add_library(copy src/task2/copy.c)

target_link_libraries(cp copy)

add_executable(runner src/task3/main.c)
add_library(graph src/task3/graph.c)

target_link_libraries(runner graph)