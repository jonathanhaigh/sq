set(SQ_RESULTS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_RESULTS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_results
    "${SQ_RESULTS_INCLUDE_DIR}/results/Filter.h"
    "${SQ_RESULTS_INCLUDE_DIR}/results/results.h"
    "${SQ_RESULTS_INCLUDE_DIR}/results/ResultView.h"
    "${SQ_RESULTS_SRC_DIR}/Filter.cpp"
    "${SQ_RESULTS_SRC_DIR}/results.cpp"
    "${SQ_RESULTS_SRC_DIR}/ResultView.cpp"
)
target_link_libraries(sq_results PUBLIC gsl)
target_link_libraries(sq_results PUBLIC sq_common_types)
target_link_libraries(sq_results PUBLIC sq_parser)
target_link_libraries(sq_results PUBLIC sq_util)

target_include_directories(sq_results PUBLIC "${SQ_RESULTS_INCLUDE_DIR}")
