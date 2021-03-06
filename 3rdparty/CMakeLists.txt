# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

# Set where we install tools/libs that are to be used on the host
set(SQ_HOST_TOOLS_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/sq-host-tools")
set(SQ_HOST_TOOLS_BIN_DIR "${SQ_HOST_TOOLS_PREFIX}/bin")
set(SQ_HOST_TOOLS_LIB_DIR "${SQ_HOST_TOOLS_PREFIX}/lib")

set(SQ_LUA_MAJOR_VER 5)
set(SQ_LUA_MINOR_VER 4)
set(SQ_LUA_PATCH_VER 2)
set(SQ_LUA_MAJMIN_VER "${SQ_LUA_MAJOR_VER}.${SQ_LUA_MINOR_VER}")
set(SQ_LUA_VER "${SQ_LUA_MAJMIN_VER}.${SQ_LUA_PATCH_VER}")
set(SQ_LUA_LIB_DIR "${SQ_HOST_TOOLS_LIB_DIR}/lua/${SQ_LUA_MAJMIN_VER}")

if (SQ_BUILD_TESTS)
    include(googletest.cmake)
endif()
include(gsl.cmake)
include(lua.cmake)
include(lua-liluat.cmake)
include(lua-lunajson.cmake)
include(range-v3.cmake)
include(rapidjson.cmake)
