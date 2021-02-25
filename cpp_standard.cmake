# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message("== Using C++ standard: ${CMAKE_CXX_STANDARD}")
if (CMAKE_CXX_EXTENSIONS)
    message("== Using C++ extensions: Yes")
else()
    message("== Using C++ extensions: No")
endif()
