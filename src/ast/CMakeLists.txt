set(SQ_AST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_AST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_ast
    "${SQ_AST_INCLUDE_DIR}/ast/ast.h"
    "${SQ_AST_SRC_DIR}/ast.cpp"
)
target_link_libraries(sq_ast PUBLIC sq_util)
target_link_libraries(sq_ast PUBLIC sq_parser)
target_link_libraries(sq_ast PUBLIC sq_schema)
target_include_directories(sq_ast PUBLIC "${SQ_AST_INCLUDE_DIR}")
