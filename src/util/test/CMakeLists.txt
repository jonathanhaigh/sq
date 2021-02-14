SET(SQ_UT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
SET(SQ_UT_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/test")
SET(SQ_UT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_util_test
    "${SQ_UT_HEADERS_DIR}/test_util.h"
    "${SQ_UT_SRC_DIR}/test_util.cpp"
)

set_target_properties(sq_util_test PROPERTIES CXX_CLANG_TIDY "")
target_include_directories(sq_util_test PUBLIC "${SQ_UT_INCLUDE_DIR}")
target_link_libraries(sq_util_test PUBLIC sq_util)
target_link_libraries(sq_util_test PUBLIC gtest)
