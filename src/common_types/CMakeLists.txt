set(SQ_COMMON_TYPES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_COMMON_TYPES_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_common_types
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/FieldCallParams.h"
    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/FieldCallParams.inl.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/FieldCallParams.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Field.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/Field.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/InvalidFieldError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/InvalidFieldError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/NotImplementedError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/NotImplementedError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/Primitive.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/Primitive.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/SqArgumentMissingError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/SqArgumentMissingError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/SqArgumentTypeError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/SqArgumentTypeError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/SqOutOfRangeError.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/SqOutOfRangeError.cpp"

    "${SQ_COMMON_TYPES_INCLUDE_DIR}/common_types/SqException.h"
    "${SQ_COMMON_TYPES_SRC_DIR}/SqException.cpp"
)
target_include_directories(sq_common_types PUBLIC "${SQ_COMMON_TYPES_INCLUDE_DIR}")
target_Link_libraries(sq_common_types PUBLIC sq_util)
target_link_libraries(sq_common_types PUBLIC range-v3)
