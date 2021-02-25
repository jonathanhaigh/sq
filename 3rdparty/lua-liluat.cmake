# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

include(ExternalProject)

ExternalProject_Add(lua_liluat
    URL https://github.com/FSMaxB/liluat/raw/v1.2.0/liluat.lua
    DOWNLOAD_NO_PROGRESS TRUE
    DOWNLOAD_NO_EXTRACT TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E make_directory
            "${SQ_LUA_LIB_DIR}"
    COMMAND
        ${CMAKE_COMMAND} -E copy
            "<DOWNLOADED_FILE>"
            "${SQ_LUA_LIB_DIR}"
)
