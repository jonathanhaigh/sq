include(FetchContent)

FetchContent_Declare(get_range_v3
    URL https://github.com/ericniebler/range-v3/archive/0.11.0.tar.gz
)
FetchContent_Populate(get_range_v3)

add_library(range_v3 INTERFACE)
add_dependencies(range_v3 get_range_v3)
target_include_directories(range_v3
    SYSTEM INTERFACE "${get_range_v3_SOURCE_DIR}/include"
)
