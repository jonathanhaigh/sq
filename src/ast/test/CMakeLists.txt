SET(SQ_ASTT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
SET(SQ_ASTT_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/test")
SET(SQ_ASTT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_ast_test
    "${SQ_ASTT_HEADERS_DIR}/test_ast.h"
    "${SQ_ASTT_SRC_DIR}/test_ast.cpp"
)

set_target_properties(sq_ast_test PROPERTIES CXX_CLANG_TIDY "")
target_include_directories(sq_ast_test PUBLIC "${SQ_ASTT_INCLUDE_DIR}")
target_link_libraries(sq_ast_test PUBLIC sq_ast)
target_link_libraries(sq_ast_test PUBLIC sq_common_types_test)
target_link_libraries(sq_ast_test PUBLIC gtest)
