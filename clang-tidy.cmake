# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

find_program(
    SQ_CLANG_TIDY_PATH
    NAMES clang-tidy clang-tidy-10 clang-tidy-11
)
if("${CMAKE_BUILD_TYPE}" STREQUAL Debug AND
   NOT CMAKE_CROSSCOMPILING AND
   SQ_CLANG_TIDY_PATH
)
    message("== Using Clang-Tidy: ${SQ_CLANG_TIDY_PATH}")
    set(CMAKE_CXX_CLANG_TIDY ${SQ_CLANG_TIDY_PATH})
else()
    message("== Using Clang-Tidy: No")
endif()
