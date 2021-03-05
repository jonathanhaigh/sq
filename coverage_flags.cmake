# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

option(SQ_USE_COVERAGE "Instrument build for test coverage analysis" FALSE)

set(SQ_COVERAGE_FLAGS)
if (SQ_USE_COVERAGE)
    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU AND
        NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang
    )
        message(FATAL_ERROR "SQ_USE_COVERAGE is set but coverage is not supported for compiler (${CMAKE_CXX_COMPILER_ID})")
    endif()
    set(SQ_COVERAGE_FLAGS
        -fno-inline
        --coverage
    )
endif()

if (SQ_COVERAGE_FLAGS)
    message("== Using coverage flags: ${SQ_COVERAGE_FLAGS}")
    add_compile_options(${SQ_COVERAGE_FLAGS})
    add_link_options(${SQ_COVERAGE_FLAGS})
else()
    message("== Using coverage flags: No")
endif()
