# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_WARNING_FLAGS)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU 
)
    set(SQ_WARNING_FLAGS
        -Wall
        -Wextra
        -pedantic-errors
        -pedantic
        -Wconversion
        -Wsign-conversion
        -Werror
        -Wold-style-cast
        -Woverloaded-virtual
    )
endif()

if (SQ_WARNING_FLAGS)
    message("== Using warning flags: ${SQ_WARNING_FLAGS}")
    add_compile_options(${SQ_WARNING_FLAGS})
else()
    message("== Using warning flags: No")
endif()
