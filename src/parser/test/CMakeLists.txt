# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

add_executable(sq-parser-test "${CMAKE_CURRENT_SOURCE_DIR}/test_parser.cpp")
set_target_properties(sq-parser-test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq-parser-test sq_parser)
target_link_libraries(sq-parser-test sq_core_test_util)
target_link_libraries(sq-parser-test gtest_main)
gtest_discover_tests(sq-parser-test)
