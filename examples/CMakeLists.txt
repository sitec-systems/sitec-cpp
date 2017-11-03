find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../can/src)
add_executable(libcan_example can_example.cpp)
target_link_libraries(libcan_example libcan ${LIBSOCKETCAN_LIBRARIES} Threads::Threads)
