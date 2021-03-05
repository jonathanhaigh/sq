# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

find_program(
    SQ_CLANG_TIDY_PATH
    NAMES clang-tidy clang-tidy-10 clang-tidy-11 clang-tidy-12
)

set(SQ_USE_CLANG_TIDY_DEFAULT FALSE)
if ("${CMAKE_BUILD_TYPE}" STREQUAL Debug AND
    NOT CMAKE_CROSSCOMPILING AND
    SQ_CLANG_TIDY_PATH
)
    set(SQ_USE_CLANG_TIDY_DEFAULT TRUE)
endif()

option(SQ_USE_CLANG_TIDY "Use Clang-Tidy for static analysis" ${SQ_USE_CLANG_TIDY_DEFAULT})

if (SQ_USE_CLANG_TIDY)
    if(NOT SQ_CLANG_TIDY_PATH)
        message(FATAL_ERROR "SQ_USE_CLANG_TIDY is set but clang-tidy was not found")
    endif()
    message("== Using Clang-Tidy: ${SQ_CLANG_TIDY_PATH}")
    set(CMAKE_CXX_CLANG_TIDY ${SQ_CLANG_TIDY_PATH})
else()
    message("== Using Clang-Tidy: No")
endif()
