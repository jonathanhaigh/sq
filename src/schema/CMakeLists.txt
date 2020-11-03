macro(list_append_prepend LIST_IN LIST_OUT PREFIX SUFFIX)
    set (${LIST_OUT} ${${LIST_IN}})
    list(TRANSFORM ${LIST_OUT} PREPEND "${PREFIX}")
    list(TRANSFORM ${LIST_OUT} APPEND "${SUFFIX}")
endmacro()

set(SQ_GEN_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
set(SQ_NOGEN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_GEN_SRC_DIR "${CMAKE_CURRENT_BINARY_DIR}/src")
set(SQ_NOGEN_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SQ_TYPES
    SqPath
    SqRoot
    SqString
)

# Generated Headers
set(SQ_TYPE_GEN_HEADERS_DIR "${SQ_GEN_INCLUDE_DIR}/field_types")

list_append_prepend(
    SQ_TYPES
    SQ_TYPE_GEN_HEADERS 
    "${SQ_TYPE_GEN_HEADERS_DIR}/"
    ".gen.h"
)

# Generated Source
set(SQ_TYPE_GEN_SRC_DIR "${SQ_GEN_SRC_DIR}/field_types")

list_append_prepend(
    SQ_TYPES
    SQ_TYPE_GEN_SRC
    "${SQ_TYPE_GEN_SRC_DIR}/"
    ".gen.cpp"
)

# Non-generated Headers
set(SQ_TYPE_NOGEN_HEADERS_DIR "${SQ_NOGEN_INCLUDE_DIR}/field_types")

list_append_prepend(
    SQ_TYPES
    SQ_TYPE_IMPL_HEADERS
    "${SQ_TYPE_NOGEN_HEADERS_DIR}/"
    "Impl.h"
)

set(SQ_TYPE_NOGEN_HEADERS
    ${SQ_TYPE_IMPL_HEADERS}
    "${SQ_TYPE_NOGEN_HEADERS_DIR}/Field.h"
    "${SQ_TYPE_NOGEN_HEADERS_DIR}/Primitive.h"
    "${SQ_TYPE_NOGEN_HEADERS_DIR}/InvalidFieldError.h"
)

# Non-generated Source
set(SQ_TYPE_NOGEN_SRC_DIR "${SQ_NOGEN_SRC_DIR}/field_types")

list_append_prepend(
    SQ_TYPES
    SQ_TYPE_IMPL_SRC
    "${SQ_TYPE_NOGEN_SRC_DIR}/"
    "Impl.cpp"
)

list_append_prepend(
    SQ_TYPES
    SQ_TYPE_GETTER_SRC
    "${SQ_TYPE_NOGEN_SRC_DIR}/"
    "_getters.cpp"
)

set(SQ_TYPE_NOGEN_SRC
    ${SQ_TYPE_GETTER_SRC}
    ${SQ_TYPE_IMPL_SRC}
    "${SQ_TYPE_NOGEN_SRC_DIR}/Field.cpp"
    "${SQ_TYPE_NOGEN_SRC_DIR}/Primitive.cpp"
    "${SQ_TYPE_NOGEN_SRC_DIR}/InvalidFieldError.cpp"
)

# Autogen command
add_custom_command(
    OUTPUT
        ${SQ_TYPE_GEN_HEADERS}
        ${SQ_TYPE_GEN_SRC}
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/generate_schema.py"
        "${SQ_TYPE_GEN_HEADERS_DIR}"
        "${SQ_TYPE_GEN_SRC_DIR}"
    DEPENDS "schema.json" "generate_schema.py"
    VERBATIM
)

# Library
add_library(sq_schema
    ${SQ_TYPE_GEN_HEADERS}
    ${SQ_TYPE_GEN_SRC}
    ${SQ_TYPE_NOGEN_HEADERS}
    ${SQ_TYPE_NOGEN_SRC}
)

set_target_properties(sq_schema PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(sq_schema PUBLIC ${SQ_GEN_INCLUDE_DIR})
target_include_directories(sq_schema PUBLIC ${SQ_NOGEN_INCLUDE_DIR})
