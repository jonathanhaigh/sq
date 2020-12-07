set(SQ_EXCEPTIONS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_EXCEPTIONS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_exceptions
    "${SQ_EXCEPTIONS_INCLUDE_DIR}/exceptions/SqException.h"
    "${SQ_EXCEPTIONS_SRC_DIR}/SqException.cpp"
)
target_include_directories(sq_exceptions PUBLIC "${SQ_EXCEPTIONS_INCLUDE_DIR}")
