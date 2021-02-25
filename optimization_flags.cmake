# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_OPTIMIZATION_FLAGS)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU
)
    if("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
        set(SQ_OPTIMIZATION_FLAGS
            -Og
            -fno-omit-frame-pointer
            -fno-optimize-sibling-calls
        )
    else()
        set(SQ_OPTIMIZATION_FLAGS
            -O2
        )
    endif()
endif()

if (SQ_OPTIMIZATION_FLAGS)
    message("== Using optimization flags: ${SQ_OPTIMIZATION_FLAGS}")
    add_compile_options(${SQ_OPTIMIZATION_FLAGS})
else()
    message("== Using optimization flags: No")
endif()
