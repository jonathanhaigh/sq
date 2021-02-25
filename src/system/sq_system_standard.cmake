# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_SYSTEM_STANDARD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_SYSTEM_STANDARD_HEADERS_DIR "${SQ_SYSTEM_STANDARD_INCLUDE_DIR}/system/standard")
set(SQ_SYSTEM_STANDARD_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/standard")

sq_expand_for_each_type(
    "${SQ_SYSTEM_STANDARD_HEADERS_DIR}/%sImpl.h"
    SQ_SYSTEM_STANDARD_HEADERS
)

sq_expand_for_each_type(
    "${SQ_SYSTEM_STANDARD_SRC_DIR}/%sImpl.cpp"
    SQ_SYSTEM_STANDARD_SRC
)

add_library(sq_system_standard
    ${SQ_SYSTEM_STANDARD_HEADERS}
    ${SQ_SYSTEM_STANDARD_SRC}
)
target_link_libraries(sq_system_standard PUBLIC sq_common_types)
target_link_libraries(sq_system_standard PUBLIC sq_util)
target_link_libraries(sq_system_standard PUBLIC sq_system_dispatch)
target_link_libraries(sq_system_standard PUBLIC gsl)
target_include_directories(sq_system_standard PUBLIC "${SQ_SYSTEM_STANDARD_INCLUDE_DIR}")
