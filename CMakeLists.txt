# Copyright 2017 sitec systems GmbH. All rights reserved
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

project(sitec-c-cpp)
cmake_minimum_required(VERSION 3.3)

option(BUILD_EXAMPLE "Build examples for the library" OFF)
option(BUILD_MACHINE_S7 "Building the library for the S7, If this option is disabled it builds for S4" OFF)
option(BUILD_SHARED "Build shared library instead off static library" OFF)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic")

if (BUILD_MACHINE_S7)
  message (STATUS "Building for S7")
else()
  message (STATUS "Building for S4")
endif(BUILD_MACHINE_S7)

add_subdirectory(can)
if(BUILD_EXAMPLE)
  add_subdirectory(examples)
endif(BUILD_EXAMPLE)

find_program(DOXYGEN_EXECUTABLE
  NAMES doxygen
  PATHS "/usr/bin"
    "/usr/local/bin"
  DOC "Tool for generatin API Documentations"
  )
if (DOXYGEN_EXECUTABLE)
  message("Found Doxygen")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf @ONlY)
  add_custom_target(doc 
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API Documentation" VERBATIM
    )
endif(DOXYGEN_EXECUTABLE)
