# Copyright 2017 sitec systems GmbH. All rights reserved
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

# - Try to find libsocketcan
# Once done it will define
#  LIBSOCKETCAN_FOUND - System has libsocketcan
#  LIBSOCKETCAN_INCLUDE_DIRS - libsocketcan include directories
#  LIBSOCKETCAN_LIBRARIES - libraries needed to use libsocketcan

find_path(LIBSOCKETCAN_INCLUDE_DIR libsocketcan.h
    PATHS 
        "${LIBSOCKETCAN_ROOT}/include"
        "$ENV{SDKTARGETSYSROOT}/usr/include"
        "/usr/include" 
        "/usr/local/include" 
    )

find_library(LIBSOCKETCAN_LIBRARY NAMES socketcan
    PATHS 
        "${LIBSOCKETCAN_ROOT}/lib"
        "$ENV{SDKTARGETSYSROOT}/usr/lib"
        "/usr/lib" 
        "/usr/local/lib" 
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libsocketcan DEFAULT_MSG
    LIBSOCKETCAN_LIBRARY LIBSOCKETCAN_INCLUDE_DIR)
mark_as_advanced(LIBSOCKETCAN_LIBRARY LIBSOCKETCAN_INCLUDE_DIR)

set(LIBSOCKETCAN_LIBRARIES ${LIBSOCKETCAN_LIBRARY})
set(LIBSOCKETCAN_INCLUDE_DIRS ${LIBSOCKETCAN_INCLUDE_DIR})
