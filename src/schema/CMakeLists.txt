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
    SqBool
    SqDataSize
    SqFloat
    SqInt
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
set(SQ_TYPE_GEN_SRC_DIR "${SQ_GEN_SRC_DIR}")

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
)

# Non-generated Source
set(SQ_TYPE_NOGEN_SRC_DIR "${SQ_NOGEN_SRC_DIR}")

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
)

# Autogen command
add_custom_command(
    OUTPUT
        ${SQ_TYPE_GEN_HEADERS}
        ${SQ_TYPE_GEN_SRC}
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/generate_schema.py"
        "${SQ_TYPE_GEN_HEADERS_DIR}"
        "${SQ_TYPE_GEN_SRC_DIR}"
    DEPENDS 
        "schema.json"
        "generate_schema.py"
        "templates/SqType.gen.h.jinja"
        "templates/SqType.gen.cpp.jinja"
    VERBATIM
)

# Library
add_library(sq_schema
    ${SQ_TYPE_GEN_HEADERS}
    ${SQ_TYPE_GEN_SRC}
    ${SQ_TYPE_NOGEN_HEADERS}
    ${SQ_TYPE_NOGEN_SRC}
)

# The generated code includes method declarations that will be defined
# manually, so it doesn't know whether those methods will actually use their
# *this objects or not. Tell clang-tidy not to suggest that those methods that
# don't use *this should be static.
set_property(
    TARGET sq_schema
    APPEND_STRING
    PROPERTY CXX_CLANG_TIDY
    ",-readability-convert-member-functions-to-static"
)

set_target_properties(sq_schema PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sq_schema PUBLIC sq_util)
target_link_libraries(sq_schema PUBLIC sq_common_types)
target_link_libraries(sq_schema PUBLIC gsl)
target_link_libraries(sq_schema PUBLIC range-v3)
target_include_directories(sq_schema PUBLIC ${SQ_GEN_INCLUDE_DIR})
target_include_directories(sq_schema PUBLIC ${SQ_NOGEN_INCLUDE_DIR})
