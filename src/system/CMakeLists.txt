set(SQ_SYSTEM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(SQ_SYSTEM_STANDARD_HEADERS_DIR "${SQ_SYSTEM_INCLUDE_DIR}/system/standard")
set(SQ_SYSTEM_STANDARD_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/standard")

add_library(sq_system_standard
    "${SQ_SYSTEM_STANDARD_HEADERS_DIR}/SqBoolImpl.h"
    "${SQ_SYSTEM_STANDARD_SRC_DIR}/SqBoolImpl.cpp"

    "${SQ_SYSTEM_STANDARD_HEADERS_DIR}/SqDataSizeImpl.h"
    "${SQ_SYSTEM_STANDARD_SRC_DIR}/SqDataSizeImpl.cpp"

    "${SQ_SYSTEM_STANDARD_HEADERS_DIR}/SqFloatImpl.h"
    "${SQ_SYSTEM_STANDARD_SRC_DIR}/SqFloatImpl.cpp"

    "${SQ_SYSTEM_STANDARD_HEADERS_DIR}/SqIntImpl.h"
    "${SQ_SYSTEM_STANDARD_SRC_DIR}/SqIntImpl.cpp"

    "${SQ_SYSTEM_STANDARD_HEADERS_DIR}/SqPathImpl.h"
    "${SQ_SYSTEM_STANDARD_SRC_DIR}/SqPathImpl.cpp"

    "${SQ_SYSTEM_STANDARD_HEADERS_DIR}/SqRootImpl.h"
    "${SQ_SYSTEM_STANDARD_SRC_DIR}/SqRootImpl.cpp"

    "${SQ_SYSTEM_STANDARD_HEADERS_DIR}/SqStringImpl.h"
    "${SQ_SYSTEM_STANDARD_SRC_DIR}/SqStringImpl.cpp"
)
target_link_libraries(sq_system_standard PUBLIC sq_common_types)
target_link_libraries(sq_system_standard PUBLIC sq_util)
target_link_libraries(sq_system_standard PUBLIC sq_schema)
target_link_libraries(sq_system_standard PUBLIC gsl)
target_include_directories(sq_system_standard PUBLIC "${SQ_SYSTEM_INCLUDE_DIR}")

set(SQ_SYSTEM_HEADERS_DIR "${SQ_SYSTEM_INCLUDE_DIR}/system")
set(SQ_SYSTEM_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_system 
    "${SQ_SYSTEM_HEADERS_DIR}/root.h"
    "${SQ_SYSTEM_SRC_DIR}/root.cpp"
)
target_link_libraries(sq_system sq_system_standard)
target_include_directories(sq_system PUBLIC "${SQ_SYSTEM_INCLUDE_DIR}")
