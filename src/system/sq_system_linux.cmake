# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

find_library(SQ_UDEV_LIB_PATH NAMES udev REQUIRED)
find_path(SQ_UDEV_INCLUDE_PATH NAMES libudev.h REQUIRED)

set(SQ_SYSTEM_LINUX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_SYSTEM_LINUX_HEADERS_DIR "${SQ_SYSTEM_LINUX_INCLUDE_DIR}/system/linux")
set(SQ_SYSTEM_LINUX_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/linux")

sq_expand_for_each_type(
    "${SQ_SYSTEM_LINUX_HEADERS_DIR}/%sImpl.h"
    SQ_SYSTEM_LINUX_TYPE_HEADERS
)

sq_expand_for_each_type(
    "${SQ_SYSTEM_LINUX_SRC_DIR}/%sImpl.cpp"
    SQ_SYSTEM_LINUX_TYPE_SRC
)

add_library(sq_system_linux
    ${SQ_SYSTEM_LINUX_TYPE_HEADERS}
    ${SQ_SYSTEM_LINUX_TYPE_SRC}
    "${SQ_SYSTEM_LINUX_HEADERS_DIR}/udev.h"
    "${SQ_SYSTEM_LINUX_HEADERS_DIR}/udev.inl.h"
    "${SQ_SYSTEM_LINUX_SRC_DIR}/udev.cpp"
)
target_link_libraries(sq_system_linux PUBLIC sq_system_schema)
target_link_libraries(sq_system_linux PUBLIC sq_common_types)
target_link_libraries(sq_system_linux PUBLIC sq_util)
target_link_libraries(sq_system_linux PUBLIC sq_system_dispatch)
target_link_libraries(sq_system_linux PUBLIC gsl)
target_link_libraries(sq_system_linux PRIVATE "${SQ_UDEV_LIB_PATH}")
target_include_directories(sq_system_linux PUBLIC "${SQ_SYSTEM_LINUX_INCLUDE_DIR}")
target_include_directories(sq_system_linux PRIVATE "${SQ_UDEV_INCLUDE_PATH}")
