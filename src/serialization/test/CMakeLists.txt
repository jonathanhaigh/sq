# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

add_executable(sq-serialization-test
    "${CMAKE_CURRENT_SOURCE_DIR}/test_serialization.cpp"
)
set_target_properties(sq-serialization-test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq-serialization-test sq_serialization)
target_link_libraries(sq-serialization-test sq_results_test_util)
target_link_libraries(sq-serialization-test gtest_main)
gtest_discover_tests(sq-serialization-test)
