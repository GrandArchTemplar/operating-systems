cmake_minimum_required(VERSION 3.1.0)
project(operating-systems VERSION 0.0.0)

include(CMakeToolsHelpers OPTIONAL)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(src/lib)
add_subdirectory(src/lib)

add_executable(cp src/task2/main.c)
add_executable(MP-runner src/task3/main.c)
add_executable(MP-matrix-summator src/task4/main.c)
add_executable(MT-runner src/task5/main.c)


target_link_libraries(cp copy)
target_link_libraries(MP-runner graph)
target_link_libraries(MT-runner graph)
target_link_libraries(MT-runner Threads::Threads)
