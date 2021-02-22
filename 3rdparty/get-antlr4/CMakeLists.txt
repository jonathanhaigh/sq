cmake_minimum_required(VERSION 3.15)

project(GetAntlr4)

include(UseJava)

if (NOT JAVA_SYMLINK_PATH)
    message(FATAL_ERROR "JAVA_SYMLINK_PATH is not set")
endif()
if (NOT ANTLR4_JAR_SYMLINK_PATH)
    message(FATAL_ERROR "ANTLR4_JAR_SYMLINK_PATH is not set")
endif()


find_package(Java COMPONENTS Runtime REQUIRED)
file(CREATE_LINK "${Java_JAVA_EXECUTABLE}" "${JAVA_SYMLINK_PATH}" SYMBOLIC)

set(ANTLR4_JAR "${CMAKE_CURRENT_BINARY_DIR}/antlr4.jar")
file(DOWNLOAD
    "https://www.antlr.org/download/antlr-4.9.1-complete.jar"
    "${ANTLR4_JAR}"
)
file(CREATE_LINK "${ANTLR4_JAR}" "${ANTLR4_JAR_SYMLINK_PATH}" SYMBOLIC)
