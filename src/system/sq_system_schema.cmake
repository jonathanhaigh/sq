# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_SYSTEM_SCHEMA_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_SYSTEM_SCHEMA_HEADERS_DIR "${SQ_SYSTEM_SCHEMA_INCLUDE_DIR}/system")
set(SQ_SYSTEM_SCHEMA_SRC_DIR "${CMAKE_CURRENT_BINARY_DIR}/src")

sq_generate_file(
    "schema.gen.cpp"
    "${SQ_SYSTEM_SCHEMA_SRC_DIR}/schema.gen.cpp"
)

add_library(sq_system_schema
    "${SQ_SYSTEM_SCHEMA_HEADERS_DIR}/schema.h"
    "${SQ_SYSTEM_SCHEMA_SRC_DIR}/schema.gen.cpp"
)

set_target_properties(sq_system_schema PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(sq_system_schema PUBLIC ${SQ_SYSTEM_SCHEMA_INCLUDE_DIR})
