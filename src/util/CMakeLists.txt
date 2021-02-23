set(SQ_UTIL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_UTIL_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_util
    "${SQ_UTIL_INCLUDE_DIR}/util/ASSERT.h"
    "${SQ_UTIL_SRC_DIR}/ASSERT.cpp"

    "${SQ_UTIL_INCLUDE_DIR}/util/MoveOnlyTree.h"
    "${SQ_UTIL_SRC_DIR}/MoveOnlyTree.cpp"

    "${SQ_UTIL_INCLUDE_DIR}/util/strutil.h"
    "${SQ_UTIL_SRC_DIR}/strutil.cpp"

    "${SQ_UTIL_INCLUDE_DIR}/util/typeutil.h"
    "${SQ_UTIL_SRC_DIR}/typeutil.cpp"
)
target_include_directories(sq_util PUBLIC "${SQ_UTIL_INCLUDE_DIR}")
target_link_libraries(sq_util PUBLIC range_v3)
target_link_libraries(sq_util PUBLIC gsl)
