# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

# The system directory is split into several libraries:
#
# * The sq_system_dispatch library that contains classes for each SQ type,
#   automatically generated from the schema. These classes handle dispatching
#   calls into the system to the implementation libraries.  Essentially, these
#   classes are for resolving SQ type field names into actual method calls.
#
# * The platform specific sq_system_linux library that provides an
#   implementation of the system for Linux.
#
# * The sq_system_schema library that contains binary schema information
#   generated from the schema JSON file.
#
# * The sq_system library that provides a handle to the system.

include(sq_functions.cmake)
include(sq_system_dispatch.cmake)
include(sq_system_linux.cmake)
include(sq_system_schema.cmake)

set(SQ_SYSTEM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_SYSTEM_HEADERS_DIR "${SQ_SYSTEM_INCLUDE_DIR}/system")
set(SQ_SYSTEM_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SQ_SYSTEM_SRC
    "${SQ_SYSTEM_HEADERS_DIR}/root.h"
    "${SQ_SYSTEM_SRC_DIR}/root.cpp"
)
add_library(sq_system ${SQ_SYSTEM_SRC})
target_link_libraries(sq_system sq_system_linux)
target_include_directories(sq_system PUBLIC "${SQ_SYSTEM_INCLUDE_DIR}")
