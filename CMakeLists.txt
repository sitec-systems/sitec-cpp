# Copyright 2017 sitec systems GmbH. All rights reserved
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

project(sitec-c-cpp)
cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(can-example ${CMAKE_CURRENT_SOURCE_DIR}/examples/can.cpp)
