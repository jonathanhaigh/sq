set(SQ_SERIALIZATION_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SQ_SERIALIZATION_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(sq_serialization
    "${SQ_SERIALIZATION_INCLUDE_DIR}/serialization/serialize.h"
    "${SQ_SERIALIZATION_SRC_DIR}/serialize.cpp"
)
target_link_libraries(sq_serialization PUBLIC rapidjson)
target_link_libraries(sq_serialization PUBLIC sq_results)
target_include_directories(sq_serialization PUBLIC "${SQ_SERIALIZATION_INCLUDE_DIR}")
