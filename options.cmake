# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

option(SQ_FORCE_COLOR "Force coloured compiler output" FALSE)
set(SQ_COLOR_FLAGS)
if("${SQ_FORCE_COLOR}" OR "$ENV{CLICOLOR_FORCE}")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU)
        list(APPEND SQ_COLOR_FLAGS "-fdiagnostics-color=always")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang)
        list(APPEND SQ_COLOR_FLAGS "-fcolor-diagnostics")
    endif()
endif()
if (SQ_COLOR_FLAGS)
    message("== Using flags to control color output: ${SQ_COLOR_FLAGS}")
    add_compile_options(${SQ_COLOR_FLAGS})
else()
    message("== Using flags to control color output: No")
endif()
