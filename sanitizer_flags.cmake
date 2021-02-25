# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_SANITIZER_FLAGS)
if("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang)
        set(SQ_SANITIZER_FLAGS
            -fsanitize=address
            -fsanitize=undefined
            -fsanitize=float-divide-by-zero
            -fsanitize=local-bounds
            -fsanitize=nullability
            -fno-sanitize-recover=undefined
            -fno-sanitize-recover=float-divide-by-zero
            -fno-sanitize-recover=local-bounds
            -fno-sanitize-recover=nullability
        )
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU)
        set(SQ_SANITIZER_FLAGS
            -fsanitize=address
            -fsanitize=pointer-compare
            -fsanitize=pointer-subtract
            -fsanitize=leak
            -fsanitize=undefined
            -fsanitize=float-divide-by-zero
            -fsanitize=float-cast-overflow
            -fno-sanitize-recover=all
            -fstack-protector-all
        )
    endif()
endif()

if (SQ_SANITIZER_FLAGS)
    message("== Using sanitizer flags: ${SQ_SANITIZER_FLAGS}")
    add_compile_options(${SQ_SANITIZER_FLAGS})
    add_link_options(${SQ_SANITIZER_FLAGS})
else()
    message("== Using sanitizer flags: No")
endif()
