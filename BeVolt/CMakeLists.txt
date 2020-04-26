cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

project(bps-simulator)

set(CMAKE_C_STANDARD 11)

add_subdirectory(Apps)
add_subdirectory(BSP)
add_subdirectory(Drivers)

