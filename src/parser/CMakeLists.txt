# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_PARSER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_PARSER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_parser
    "${SQ_PARSER_INCLUDE_DIR}/parser/Ast.h"
    "${SQ_PARSER_SRC_DIR}/Ast.cpp"

    "${SQ_PARSER_INCLUDE_DIR}/parser/FilterSpec.h"
    "${SQ_PARSER_SRC_DIR}/FilterSpec.cpp"

    "${SQ_PARSER_INCLUDE_DIR}/parser/Parser.h"
    "${SQ_PARSER_INCLUDE_DIR}/parser/Parser.inl.h"
    "${SQ_PARSER_SRC_DIR}/Parser.cpp"

    "${SQ_PARSER_INCLUDE_DIR}/parser/TokenView.h"
    "${SQ_PARSER_SRC_DIR}/TokenView.cpp"
)
target_include_directories(sq_parser PUBLIC "${SQ_PARSER_INCLUDE_DIR}")
target_link_libraries(sq_parser PUBLIC sq_util)
target_link_libraries(sq_parser PUBLIC sq_common_types)
target_link_libraries(sq_parser PUBLIC range_v3)
