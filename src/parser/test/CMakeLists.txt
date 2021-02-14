set(SQ_PT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_PT_HEADERS_DIR "${SQ_PT_INCLUDE_DIR}/test")
set(SQ_PT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_parser_test
    "${SQ_PT_HEADERS_DIR}/test_parser.h"
    "${SQ_PT_SRC_DIR}/test_parser.cpp"
)
target_include_directories(sq_parser_test PUBLIC "${SQ_PT_INCLUDE_DIR}")
set_target_properties(sq_parser_test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq_parser_test PUBLIC gtest)
