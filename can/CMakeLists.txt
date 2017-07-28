project(libcan)
cmake_minimum_required(VERSION 3.3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic")

find_package(Libsocketcan)

include_directories(${LIBSOCKETCAN_INLCUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

if (BUILD_SHARED)
  add_library(libcan SHARED 
      ${CMAKE_CURRENT_SOURCE_DIR}/src/can.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/can_frame.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/can_filter.cpp
      )
else()
  add_library(libcan STATIC
      ${CMAKE_CURRENT_SOURCE_DIR}/src/can.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/can_frame.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/can_filter.cpp
      )
endif(BUILD_SHARED)

if (BUILD_MACHINE_S7)
  add_library(libinterface_configuration STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/s7/interface_configuration.cpp
    )
else()
  add_library(libinterface_configuration STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/s4/interface_configuration.cpp
    )
endif(BUILD_MACHINE_S7)


target_link_libraries(libcan ${LIBSOCKETCAN_LIBRARIES} libinterface_configuration)

if (BUILD_EXAMPLE)
    find_package(Threads REQUIRED)
    add_executable(libcan_example example/example.cpp ${BACKWARD_ENABLE})
    target_link_libraries(libcan_example libcan ${LIBSOCKETCAN_LIBRARIES} Threads::Threads)
    add_backward(libcan_example)
endif(BUILD_EXAMPLE)

