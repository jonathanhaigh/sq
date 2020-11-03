set(SQ_UTIL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_UTIL_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_util
    "${SQ_UTIL_INCLUDE_DIR}/util/Tree.h"
    "${SQ_UTIL_SRC_DIR}/Tree.cpp"
)
target_include_directories(sq_util PUBLIC "${SQ_UTIL_INCLUDE_DIR}")
