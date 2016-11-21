cmake_minimum_required(VERSION 3.0.0)
project(operating-systems VERSION 0.0.0)

include(CMakeToolsHelpers OPTIONAL)



include_directories(src/lib)
add_subdirectory(src/lib)

add_executable(cp src/task2/main.c)
add_executable(runner src/task3/main.c)

target_link_libraries(cp copy)
target_link_libraries(runner graph)