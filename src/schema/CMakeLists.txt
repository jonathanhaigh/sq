set(SQ_SCHEMA_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
set(SQ_SCHEMA_HEADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/schema")
set(SQ_SCHEMA_SRC_DIR "${CMAKE_CURRENT_BINARY_DIR}/src")

set(SQ_SCHEMA_HEADERS
    "${SQ_SCHEMA_HEADERS_DIR}/SqBool.gen.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqBool.gen.inl.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqDataSize.gen.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqDataSize.gen.inl.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqFloat.gen.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqFloat.gen.inl.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqInt.gen.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqInt.gen.inl.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqPath.gen.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqPath.gen.inl.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqRoot.gen.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqRoot.gen.inl.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqString.gen.h"
    "${SQ_SCHEMA_HEADERS_DIR}/SqString.gen.inl.h"
)
set(SQ_SCHEMA_SRC
    "${SQ_SCHEMA_SRC_DIR}/SqBool.gen.cpp"
    "${SQ_SCHEMA_SRC_DIR}/SqDataSize.gen.cpp"
    "${SQ_SCHEMA_SRC_DIR}/SqFloat.gen.cpp"
    "${SQ_SCHEMA_SRC_DIR}/SqInt.gen.cpp"
    "${SQ_SCHEMA_SRC_DIR}/SqPath.gen.cpp"
    "${SQ_SCHEMA_SRC_DIR}/SqRoot.gen.cpp"
    "${SQ_SCHEMA_SRC_DIR}/SqString.gen.cpp"
)

add_custom_command(
    OUTPUT
        ${SQ_SCHEMA_HEADERS}
        ${SQ_SCHEMA_SRC}
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/generate_schema.py"
        "${SQ_SCHEMA_HEADERS_DIR}"
        "${SQ_SCHEMA_SRC_DIR}"
    DEPENDS 
        "schema.json"
        "generate_schema.py"
        "templates/SqType.gen.h.jinja"
        "templates/SqType.gen.inl.h.jinja"
        "templates/SqType.gen.cpp.jinja"
    VERBATIM
)

add_library(sq_schema ${SQ_SCHEMA_HEADERS} ${SQ_SCHEMA_SRC})

set_target_properties(sq_schema PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sq_schema PUBLIC sq_common_types)
target_include_directories(sq_schema PUBLIC ${SQ_SCHEMA_INCLUDE_DIR})
