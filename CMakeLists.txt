project(sitec-C-Cpp-Lib)
cmake_minimum_required(VERSION 3.3)

option(BUILD_EXAMPLE "Build examples for the library" ON)
option(BUILD_MACHINE_S7 "Building the library for the S7, If this option is disabled it builds for S4" OFF)
option(BUILD_SHARED "Build shared library instead off static library" OFF)

set(CMAKE_CXX_STANDARD 14)

if (BUILD_MACHINE_S7)
  message (STATUS "Building for S7")
else()
  message (STATUS "Building for S4")
endif(BUILD_MACHINE_S7)

include_directories(include)

add_subdirectory(3rdparty)
add_subdirectory(can)
