set(SQ_COMMON_TYPES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_COMMON_TYPES_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_common_types
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/ArgumentMissingError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/ArgumentMissingError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/ArgumentTypeError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/ArgumentTypeError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Exception.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/Exception.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/FieldCallParams.h"
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/FieldCallParams.inl.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/FieldCallParams.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Field.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/Field.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/InternalError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/InternalError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/InvalidFieldError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/InvalidFieldError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/NotAnArrayError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/NotAnArrayError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/NotImplementedError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/NotImplementedError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/OutOfRangeError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/OutOfRangeError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Primitive.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/Primitive.cpp"
)
target_include_directories(sq_common_types PUBLIC "${SQ_COMMON_TYPES_INCLUDE_DIR}")
target_Link_libraries(sq_common_types PUBLIC sq_util)
target_link_libraries(sq_common_types PUBLIC range_v3)
