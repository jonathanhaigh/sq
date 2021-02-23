include(FetchContent)

FetchContent_Declare(get_rapidjson
    URL https://github.com/Tencent/rapidjson/archive/b1a4d91a53e0884ff7f60eafd9e9efa98781c97b.tar.gz
    DOWNLOAD_NO_PROGRESS TRUE
)
FetchContent_Populate(get_rapidjson)

add_library(rapidjson INTERFACE)
add_dependencies(rapidjson get_rapidjson)
target_include_directories(rapidjson
    SYSTEM INTERFACE "${get_rapidjson_SOURCE_DIR}/include"
)
