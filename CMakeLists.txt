cmake_minimum_required(VERSION 3.4)
project(libhoststat C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
