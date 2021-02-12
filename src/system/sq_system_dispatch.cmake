set(SQ_SYSTEM_DISPATCH_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
set(SQ_SYSTEM_DISPATCH_HEADERS_DIR "${SQ_SYSTEM_DISPATCH_INCLUDE_DIR}/system")
set(SQ_SYSTEM_DISPATCH_SRC_DIR "${CMAKE_CURRENT_BINARY_DIR}/src")

set(SQ_SYSTEM_DISPATCH_HEADERS
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqBool.gen.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqBool.gen.inl.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqDataSize.gen.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqDataSize.gen.inl.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqFloat.gen.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqFloat.gen.inl.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqInt.gen.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqInt.gen.inl.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqPath.gen.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqPath.gen.inl.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqRoot.gen.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqRoot.gen.inl.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqString.gen.h"
    "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}/SqString.gen.inl.h"
)
set(SQ_SYSTEM_DISPATCH_SRC
    "${SQ_SYSTEM_DISPATCH_SRC_DIR}/SqBool.gen.cpp"
    "${SQ_SYSTEM_DISPATCH_SRC_DIR}/SqDataSize.gen.cpp"
    "${SQ_SYSTEM_DISPATCH_SRC_DIR}/SqFloat.gen.cpp"
    "${SQ_SYSTEM_DISPATCH_SRC_DIR}/SqInt.gen.cpp"
    "${SQ_SYSTEM_DISPATCH_SRC_DIR}/SqPath.gen.cpp"
    "${SQ_SYSTEM_DISPATCH_SRC_DIR}/SqRoot.gen.cpp"
    "${SQ_SYSTEM_DISPATCH_SRC_DIR}/SqString.gen.cpp"
)

add_custom_command(
    OUTPUT
        ${SQ_SYSTEM_DISPATCH_HEADERS}
        ${SQ_SYSTEM_DISPATCH_SRC}
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/generate_system_dispatch.py"
        "${SQ_SYSTEM_DISPATCH_HEADERS_DIR}"
        "${SQ_SYSTEM_DISPATCH_SRC_DIR}"
    DEPENDS
        "schema.json"
        "generate_system_dispatch.py"
        "templates/SqType.gen.h.jinja"
        "templates/SqType.gen.inl.h.jinja"
        "templates/SqType.gen.cpp.jinja"
    VERBATIM
)

add_library(sq_system_dispatch ${SQ_SYSTEM_DISPATCH_HEADERS} ${SQ_SYSTEM_DISPATCH_SRC})

set_target_properties(sq_system_dispatch PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(sq_system_dispatch PUBLIC sq_common_types)
target_include_directories(sq_system_dispatch PUBLIC ${SQ_SYSTEM_DISPATCH_INCLUDE_DIR})
