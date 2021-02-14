set(SQ_CTT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_CTT_HEADERS_DIR "${SQ_CTT_INCLUDE_DIR}/test")
set(SQ_CTT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_common_types_test
    "${SQ_CTT_HEADERS_DIR}/test_common_types.h"
    "${SQ_CTT_SRC_DIR}/test_common_types.cpp"

    "${SQ_CTT_SRC_DIR}/FieldCallParams_test_util.cpp"
    "${SQ_CTT_HEADERS_DIR}/FieldCallParams_test_util.h"
    "${SQ_CTT_HEADERS_DIR}/FieldCallParams_test_util.inl.h"

    "${SQ_CTT_SRC_DIR}/Primitive_test_util.cpp"
    "${SQ_CTT_HEADERS_DIR}/Primitive_test_util.h"
    "${SQ_CTT_HEADERS_DIR}/Primitive_test_util.inl.h"
)
target_include_directories(sq_common_types_test PUBLIC "${SQ_CTT_INCLUDE_DIR}")
set_target_properties(sq_common_types_test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq_common_types_test PUBLIC sq_common_types)
target_link_libraries(sq_common_types_test PUBLIC gtest)
