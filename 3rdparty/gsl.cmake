# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

include(FetchContent)

FetchContent_Declare(get_gsl
    URL https://github.com/microsoft/GSL/archive/v3.1.0.tar.gz
    DOWNLOAD_NO_PROGRESS TRUE
)
FetchContent_Populate(get_gsl)

add_library(gsl INTERFACE)
add_dependencies(gsl get_gsl)
target_include_directories(gsl
    SYSTEM INTERFACE "${get_gsl_SOURCE_DIR}/include"
)
