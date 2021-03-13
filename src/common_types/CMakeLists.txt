# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_COMMON_TYPES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_COMMON_TYPES_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_common_types
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/errors.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/errors.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/FieldCallParams.h"
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/FieldCallParams.inl.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/FieldCallParams.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Field.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/Field.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Primitive.fwd.h"
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Primitive.h"
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Primitive.inl.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/Primitive.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Token.fwd.h"
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Token.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/Token.cpp"
)
target_include_directories(sq_common_types PUBLIC "${SQ_COMMON_TYPES_INCLUDE_DIR}")
target_Link_libraries(sq_common_types PUBLIC sq_util)
target_link_libraries(sq_common_types PUBLIC range_v3)
target_link_libraries(sq_common_types PUBLIC gsl)
target_Link_libraries(sq_common_types PUBLIC fmt)
