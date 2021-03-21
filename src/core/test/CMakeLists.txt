# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_CTT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_CTT_HEADERS_DIR "${SQ_CTT_INCLUDE_DIR}/test")
set(SQ_CTT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_core_test_util
    "${SQ_CTT_SRC_DIR}/FieldCallParams_test_util.cpp"
    "${SQ_CTT_HEADERS_DIR}/FieldCallParams_test_util.h"
    "${SQ_CTT_HEADERS_DIR}/FieldCallParams_test_util.inl.h"

    "${SQ_CTT_SRC_DIR}/Primitive_test_util.cpp"
    "${SQ_CTT_HEADERS_DIR}/Primitive_test_util.h"
)
set_target_properties(sq_core_test_util PROPERTIES CXX_CLANG_TIDY "")
target_include_directories(
    sq_core_test_util PUBLIC "${SQ_CTT_INCLUDE_DIR}"
)
target_link_libraries(sq_core_test_util PUBLIC sq_core)
target_link_libraries(sq_core_test_util PUBLIC gtest)

add_executable(sq-core-test "${SQ_CTT_SRC_DIR}/test_core.cpp")
set_target_properties(sq-core-test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq-core-test sq_core_test_util)
target_link_libraries(sq-core-test gtest_main)
gtest_discover_tests(sq-core-test)
