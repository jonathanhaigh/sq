set(SQ_AST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_AST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_ast
    "${SQ_AST_INCLUDE_DIR}/ast/ast.h"
    "${SQ_AST_SRC_DIR}/ast.cpp"
    "${SQ_AST_INCLUDE_DIR}/ast/FilterSpec.h"
    "${SQ_AST_SRC_DIR}/FilterSpec.cpp"
    "${SQ_AST_INCLUDE_DIR}/ast/ParseError.h"
    "${SQ_AST_SRC_DIR}/ParseError.cpp"
)
target_link_libraries(sq_ast PUBLIC sq_exceptions)
target_link_libraries(sq_ast PUBLIC sq_util)
target_link_libraries(sq_ast PUBLIC sq_parser)
target_link_libraries(sq_ast PUBLIC sq_schema)
target_include_directories(sq_ast PUBLIC "${SQ_AST_INCLUDE_DIR}")
