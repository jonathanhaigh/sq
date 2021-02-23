include(ExternalProject)

ExternalProject_Add(lua_lunajson
    URL https://github.com/grafi-tt/lunajson/archive/1.2.3.tar.gz
    DOWNLOAD_NO_PROGRESS TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E make_directory
            "${SQ_LUA_LIB_DIR}"
    COMMAND
        ${CMAKE_COMMAND} -E copy
            "<SOURCE_DIR>/src/lunajson.lua"
            "${SQ_LUA_LIB_DIR}"
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory
            "<SOURCE_DIR>/src/lunajson"
            "${SQ_LUA_LIB_DIR}/lunajson"
)
