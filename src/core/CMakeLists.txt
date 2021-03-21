# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_CORE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_CORE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_core
    "${SQ_CORE_INCLUDE_DIR}/core/ASSERT.h"
    "${SQ_CORE_SRC_DIR}/ASSERT.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/errors.h"
    "${SQ_CORE_SRC_DIR}/errors.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/FieldCallParams.h"
    "${SQ_CORE_INCLUDE_DIR}/core/FieldCallParams.inl.h"
    "${SQ_CORE_SRC_DIR}/FieldCallParams.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/Field.h"
    "${SQ_CORE_SRC_DIR}/Field.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/MoveOnlyTree.h"
    "${SQ_CORE_SRC_DIR}/MoveOnlyTree.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/narrow.h"
    "${SQ_CORE_INCLUDE_DIR}/core/narrow.inl.h"
    "${SQ_CORE_SRC_DIR}/narrow.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/Primitive.fwd.h"
    "${SQ_CORE_INCLUDE_DIR}/core/Primitive.h"
    "${SQ_CORE_INCLUDE_DIR}/core/Primitive.inl.h"
    "${SQ_CORE_SRC_DIR}/Primitive.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/strutil.h"
    "${SQ_CORE_SRC_DIR}/strutil.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/Token.fwd.h"
    "${SQ_CORE_INCLUDE_DIR}/core/Token.h"
    "${SQ_CORE_SRC_DIR}/Token.cpp"

    "${SQ_CORE_INCLUDE_DIR}/core/typeutil.h"
    "${SQ_CORE_INCLUDE_DIR}/core/typeutil.inl.h"
    "${SQ_CORE_SRC_DIR}/typeutil.cpp"
)
target_include_directories(sq_core PUBLIC "${SQ_CORE_INCLUDE_DIR}")
target_link_libraries(sq_core PUBLIC range_v3)
target_link_libraries(sq_core PUBLIC gsl)
target_Link_libraries(sq_core PUBLIC fmt)
