# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

add_executable(sq-util-test "${CMAKE_CURRENT_SOURCE_DIR}/test_util.cpp")
set_target_properties(sq-util-test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq-util-test sq_util)
target_link_libraries(sq-util-test gtest_main)
gtest_discover_tests(sq-util-test)
