include(ExternalProject)

set(SQ_LUA_PATH_ARG "-DLUA_PATH=${SQ_LUA_PATH}")

ExternalProject_Add(lua_project
    URL https://www.lua.org/ftp/lua-${SQ_LUA_VER}.tar.gz
    INSTALL_DIR "${SQ_HOST_TOOLS_PREFIX}"
    CONFIGURE_COMMAND ""
    PATCH_COMMAND
        ${CMAKE_COMMAND}
            -D "SQ_LUACONF_PATH=<SOURCE_DIR>/src/luaconf.h"
            -D "SQ_HOST_TOOLS_PREFIX=${SQ_HOST_TOOLS_PREFIX}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/set_lua_root.cmake"
    BUILD_COMMAND make
    BUILD_IN_SOURCE TRUE
    INSTALL_COMMAND make install INSTALL_TOP="${SQ_HOST_TOOLS_PREFIX}"
)

add_executable(lua IMPORTED GLOBAL)
add_dependencies(lua lua_project)
set_target_properties(lua PROPERTIES IMPORTED_LOCATION "${SQ_HOST_TOOLS_BIN_DIR}/lua")

add_executable(luac IMPORTED GLOBAL)
add_dependencies(luac lua_project)
set_target_properties(luac PROPERTIES IMPORTED_LOCATION "${SQ_HOST_TOOLS_BIN_DIR}/luac")
