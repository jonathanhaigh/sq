SET(SQ_RT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
SET(SQ_RT_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/test")
set(SQ_RT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(sq_results_test
    "${SQ_RT_HEADERS_DIR}/test_results.h"
    "${SQ_RT_SRC_DIR}/test_results.cpp"

    "${SQ_RT_HEADERS_DIR}/results_test_util.h"
    "${SQ_RT_HEADERS_DIR}/results_test_util.inl.h"
    "${SQ_RT_SRC_DIR}/results_test_util.cpp"
)

set_target_properties(sq_results_test PROPERTIES CXX_CLANG_TIDY "")
target_include_directories(sq_results_test PUBLIC "${SQ_RT_INCLUDE_DIR}")
target_link_libraries(sq_results_test PUBLIC sq_results)
target_link_libraries(sq_results_test PUBLIC sq_common_types_test)
target_link_libraries(sq_results_test PUBLIC sq_serialization)
target_link_libraries(sq_results_test PUBLIC gtest)
target_link_libraries(sq_results_test PUBLIC gmock)
