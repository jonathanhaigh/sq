# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

include(FetchContent)

FetchContent_Declare(get_range_v3
    URL https://github.com/jonathanhaigh/range-v3/archive/jh-cache1-ubsan.tar.gz
    DOWNLOAD_NO_PROGRESS TRUE
)
FetchContent_Populate(get_range_v3)

add_library(range_v3 INTERFACE)
add_dependencies(range_v3 get_range_v3)
target_include_directories(range_v3
    SYSTEM INTERFACE "${get_range_v3_SOURCE_DIR}/include"
)
