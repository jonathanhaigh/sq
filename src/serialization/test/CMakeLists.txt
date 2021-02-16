set(SQ_ST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_ST_HEADERS_DIR "${SQ_ST_INCLUDE_DIR}/test")
set(SQ_ST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_serialization_test
    "${SQ_ST_HEADERS_DIR}/test_serialization.h"
    "${SQ_ST_SRC_DIR}/test_serialization.cpp"
)
target_include_directories(sq_serialization_test PUBLIC "${SQ_ST_INCLUDE_DIR}")
set_target_properties(sq_serialization_test PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(sq_serialization_test PUBLIC sq_serialization)
target_link_libraries(sq_serialization_test PUBLIC sq_results_test)
target_link_libraries(sq_serialization_test PUBLIC gtest)
