# Copyright 2017 sitec systems GmbH. All rights reserved
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.
find_package(Libsocketcan)

include_directories(${LIBSOCKETCAN_INLCUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SITEC_CPP_CAN_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (BUILD_SHARED)
  add_library(libcan SHARED 
      ${CMAKE_CURRENT_SOURCE_DIR}/can.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/can_frame.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/can_filter.cpp
      )
else()
  add_library(libcan STATIC
      ${CMAKE_CURRENT_SOURCE_DIR}/can.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/can_frame.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/can_filter.cpp
      )
endif(BUILD_SHARED)

if (BUILD_MACHINE_S7)
  add_library(libinterface_configuration STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/s7/interface_configuration.cpp
    )
else()
  add_library(libinterface_configuration STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/s4/interface_configuration.cpp
    )
endif(BUILD_MACHINE_S7)

target_link_libraries(libinterface_configuration ${LIBSOCKETCAN_LIBRARIES})
target_link_libraries(libcan libinterface_configuration)
