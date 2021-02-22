include(FetchContent)

FetchContent_Declare(get_rapidjson
    URL https://github.com/Tencent/rapidjson/archive/v1.1.0.tar.gz
)
FetchContent_Populate(get_rapidjson)

add_library(rapidjson INTERFACE)
add_dependencies(rapidjson get_rapidjson)
target_include_directories(rapidjson
    SYSTEM INTERFACE "${get_rapidjson_SOURCE_DIR}/include"
)
