# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_SYSTEM_DISPATCH_GEN_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
set(SQ_SYSTEM_DISPATCH_GEN_HEADERS_DIR "${SQ_SYSTEM_DISPATCH_GEN_INCLUDE_DIR}/system")
set(SQ_SYSTEM_DISPATCH_GEN_SRC_DIR "${CMAKE_CURRENT_BINARY_DIR}/src")

set(SQ_SYSTEM_DISPATCH_NOGEN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_SYSTEM_DISPATCH_NOGEN_HEADERS_DIR "${SQ_SYSTEM_DISPATCH_NOGEN_INCLUDE_DIR}/system")
set(SQ_SYSTEM_DISPATCH_NOGEN_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

sq_generate_file_for_each_type(
    "SqType.gen.h"
    "${SQ_SYSTEM_DISPATCH_GEN_HEADERS_DIR}/%s.gen.h"
    SQ_SYSTEM_DISPATCH_GEN_HEADERS
)
sq_generate_file_for_each_type(
    "SqType.gen.inl.h"
    "${SQ_SYSTEM_DISPATCH_GEN_HEADERS_DIR}/%s.gen.inl.h"
    SQ_SYSTEM_DISPATCH_GEN_INL_HEADERS
)
sq_generate_file_for_each_type(
    "SqType.gen.cpp"
    "${SQ_SYSTEM_DISPATCH_GEN_SRC_DIR}/%s.gen.cpp"
    SQ_SYSTEM_DISPATCH_GEN_SRC
)

add_library(sq_system_dispatch
    "${SQ_SYSTEM_DISPATCH_NOGEN_SRC_DIR}/CacheingField.cpp"
    "${SQ_SYSTEM_DISPATCH_NOGEN_HEADERS_DIR}/CacheingField.h"
    ${SQ_SYSTEM_DISPATCH_GEN_HEADERS}
    ${SQ_SYSTEM_DISPATCH_GEN_INL_HEADERS}
    ${SQ_SYSTEM_DISPATCH_GEN_SRC}
)

set_target_properties(sq_system_dispatch PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sq_system_dispatch PUBLIC sq_common_types)
target_include_directories(sq_system_dispatch PUBLIC ${SQ_SYSTEM_DISPATCH_GEN_INCLUDE_DIR})
target_include_directories(sq_system_dispatch PUBLIC ${SQ_SYSTEM_DISPATCH_NOGEN_INCLUDE_DIR})
