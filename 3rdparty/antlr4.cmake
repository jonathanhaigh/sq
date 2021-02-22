include(ExternalProject)

# We need the ANTLR4 tool to run on the host machine.
#
# We find or download the antlr4 tool in an external project because if we're
# cross compiling then an external project will still look for the tool (and
# the Java runtime) on the host rather than in a sysroot for the target
# platform.

set(ANTLR4_JAR_SYMLINK_PATH "${CMAKE_CURRENT_BINARY_DIR}/antlr4.jar")
set(JAVA_SYMLINK_PATH "${CMAKE_CURRENT_BINARY_DIR}/java")

ExternalProject_Add(get_antrl4
    URL "${CMAKE_CURRENT_SOURCE_DIR}/get-antlr4"
    CMAKE_ARGS
        "-DANTLR4_JAR_SYMLINK_PATH=${ANTLR4_JAR_SYMLINK_PATH}"
        "-DJAVA_SYMLINK_PATH=${JAVA_SYMLINK_PATH}"
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

add_library(antlr4_jar UNKNOWN IMPORTED GLOBAL)
add_dependencies(antlr4_jar get_antrl4)
set_target_properties(antlr4_jar
    PROPERTIES
    IMPORTED_LOCATION "${ANTLR4_JAR_SYMLINK_PATH}"
)

add_executable(java_exe IMPORTED GLOBAL)
add_dependencies(java_exe get_antrl4)
set_target_properties(java_exe
    PROPERTIES
    IMPORTED_LOCATION "${JAVA_SYMLINK_PATH}"
)
