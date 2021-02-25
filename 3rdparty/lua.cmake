# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

include(ExternalProject)

set(SQ_LUA_MAKE_CONFIG
    "INSTALL_TOP=${SQ_HOST_TOOLS_PREFIX}"
)

# We're building lua for the host so we only want to forward CMake's build
# settings on to the Lua build if those build settings are also for the host.
# If CMake is cross compiling then just use the default Lua build settings.
if(NOT CMAKE_CROSSCOMPILING)
    list(APPEND SQ_LUA_MAKE_CONFIG "CC=${CMAKE_C_COMPILER}")
    list(APPEND SQ_LUA_MAKE_CONFIG "MYCFLAGS=${CMAKE_C_FLAGS}")
    list(APPEND SQ_LUA_MAKE_CONFIG "MYLDFLAGS=${CMAKE_EXE_LINKER_FLAGS}")
endif()

ExternalProject_Add(lua_project
    URL https://www.lua.org/ftp/lua-${SQ_LUA_VER}.tar.gz
    BUILD_BYPRODUCTS
        "${SQ_HOST_TOOLS_BIN_DIR}/lua"
        "${SQ_HOST_TOOLS_BIN_DIR}/luac"
    DOWNLOAD_NO_PROGRESS TRUE
    INSTALL_DIR "${SQ_HOST_TOOLS_PREFIX}"
    CONFIGURE_COMMAND
        ${CMAKE_COMMAND}
            -D "SQ_LUACONF_PATH=<SOURCE_DIR>/src/luaconf.h"
            -D "SQ_HOST_TOOLS_PREFIX=${SQ_HOST_TOOLS_PREFIX}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/set_lua_root.cmake"
    BUILD_COMMAND
        make
            ${SQ_LUA_MAKE_CONFIG}
    BUILD_IN_SOURCE TRUE
    INSTALL_COMMAND
        make
            install
            ${SQ_LUA_MAKE_CONFIG}
)

add_executable(lua IMPORTED GLOBAL)
add_dependencies(lua lua_project)
set_target_properties(lua PROPERTIES IMPORTED_LOCATION "${SQ_HOST_TOOLS_BIN_DIR}/lua")

add_executable(luac IMPORTED GLOBAL)
add_dependencies(luac lua_project)
set_target_properties(luac PROPERTIES IMPORTED_LOCATION "${SQ_HOST_TOOLS_BIN_DIR}/luac")
