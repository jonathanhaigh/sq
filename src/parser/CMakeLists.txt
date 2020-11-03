find_path(ANTLR4_CPP_RUNTIME_INCLUDE_DIR
    NAMES "antlr4-runtime.h"
    PATH_SUFFIXES "antlr4-runtime"
    DOC "Path to the ANTLR4 C++ runtime include directory"
)

if (NOT ANTLR4_CPP_RUNTIME_INCLUDE_DIR)
    message(FATAL_ERROR "Failed to find ANTLR4 C++ runtime include directory")
endif()

set(SQ_ANTLR4_GEN_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
set(SQ_ANTLR4_GEN_DIR "${SQ_ANTLR4_GEN_INCLUDE_DIR}/parser")

set(SQ_ANTLR4_GEN_HEADERS
    "${SQ_ANTLR4_GEN_DIR}/SqBaseVisitor.h"
    "${SQ_ANTLR4_GEN_DIR}/SqLexer.h"
    "${SQ_ANTLR4_GEN_DIR}/SqVisitor.h"
    "${SQ_ANTLR4_GEN_DIR}/SqParser.h"
)

set (SQ_ANTLR4_GEN_SRC
    "${SQ_ANTLR4_GEN_DIR}/SqBaseVisitor.cpp"
    "${SQ_ANTLR4_GEN_DIR}/SqLexer.cpp"
    "${SQ_ANTLR4_GEN_DIR}/SqVisitor.cpp"
    "${SQ_ANTLR4_GEN_DIR}/SqParser.cpp"
)

add_custom_command(
    OUTPUT ${SQ_ANTLR4_GEN_HEADERS} ${SQ_ANTLR4_GEN_SRC}
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${SQ_ANTLR4_GEN_DIR}"
    COMMAND "antlr4"
        -o "${SQ_ANTLR4_GEN_DIR}"
        -Dlanguage=Cpp
        -Werror
        -no-listener
        -visitor
        -package "sq::parser"
        "${CMAKE_CURRENT_SOURCE_DIR}/Sq.g4"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Sq.g4"
    VERBATIM
)

add_library(sq_parser
    ${SQ_ANTLR4_GEN_HEADERS}
    ${SQ_ANTLR4_GEN_SRC}
)

target_include_directories(sq_parser PUBLIC ${SQ_ANTLR4_GEN_INCLUDE_DIR})

# TODO: this should be PRIVATE
target_include_directories(sq_parser PUBLIC ${ANTLR4_CPP_RUNTIME_INCLUDE_DIR})

target_link_libraries(sq_parser PUBLIC antlr4-runtime)
