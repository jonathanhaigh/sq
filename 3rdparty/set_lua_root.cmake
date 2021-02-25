# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

# Set the LUA_ROOT macro (used to generate DEFAULT_LUA_PATH) in the Lua config
# header file so that lua looks for libraries in our build environment rather
# than the host system.

file(READ "${SQ_LUACONF_PATH}" SQ_LUACONF)
string(REGEX REPLACE
    "#define[ \t]+LUA_ROOT[ \t]+[^\n]+\n"
    "#define LUA_ROOT \"${SQ_HOST_TOOLS_PREFIX}/\"\n"
    SQ_LUACONF
    "${SQ_LUACONF}"
)
file(WRITE "${SQ_LUACONF_PATH}" "${SQ_LUACONF}")
