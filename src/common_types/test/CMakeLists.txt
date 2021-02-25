# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_CTT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_CTT_HEADERS_DIR "${SQ_CTT_INCLUDE_DIR}/test")
set(SQ_CTT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_common_types_test_util
    "${SQ_CTT_SRC_DIR}/FieldCallParams_test_util.cpp"
    "${SQ_CTT_HEADERS_DIR}/FieldCallParams_test_util.h"
    "${SQ_CTT_HEADERS_DIR}/FieldCallParams_test_util.inl.h"

    "${SQ_CTT_SRC_DIR}/Primitive_test_util.cpp"
    "${SQ_CTT_HEADERS_DIR}/Primitive_test_util.h"
)
set_target_properties(sq_common_types_test_util PROPERTIES CXX_CLANG_TIDY "")
target_include_directories(
    sq_common_types_test_util PUBLIC "${SQ_CTT_INCLUDE_DIR}"
)
target_link_libraries(sq_common_types_test_util PUBLIC sq_common_types)
target_link_libraries(sq_common_types_test_util PUBLIC gtest)

add_executable(sq-common-types-test "${SQ_CTT_SRC_DIR}/test_common_types.cpp")
set_target_properties(sq-common-types-test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq-common-types-test sq_common_types_test_util)
target_link_libraries(sq-common-types-test gtest_main)
gtest_discover_tests(sq-common-types-test)
