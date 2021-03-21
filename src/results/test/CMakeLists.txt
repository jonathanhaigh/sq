# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

SET(SQ_RT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
SET(SQ_RT_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/test")
set(SQ_RT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_results_test_util
    "${SQ_RT_HEADERS_DIR}/results_test_util.h"
    "${SQ_RT_HEADERS_DIR}/results_test_util.inl.h"
    "${SQ_RT_SRC_DIR}/results_test_util.cpp"
)

set_target_properties(sq_results_test_util PROPERTIES CXX_CLANG_TIDY "")
target_include_directories(sq_results_test_util PUBLIC "${SQ_RT_INCLUDE_DIR}")
target_link_libraries(sq_results_test_util PUBLIC sq_results)
target_link_libraries(sq_results_test_util PUBLIC sq_serialization)
target_link_libraries(sq_results_test_util PUBLIC sq_core_test_util)
target_link_libraries(sq_results_test_util PUBLIC gtest)
target_link_libraries(sq_results_test_util PUBLIC gmock)

add_executable(sq-results-test "${SQ_RT_SRC_DIR}/test_results.cpp")
set_target_properties(sq-results-test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq-results-test sq_results_test_util)
target_link_libraries(sq-results-test gtest_main)
gtest_discover_tests(sq-results-test)
