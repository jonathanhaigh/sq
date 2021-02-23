include(FetchContent)

FetchContent_Declare(get_googletest
    URL https://github.com/google/googletest/archive/v1.10.x.tar.gz
    DOWNLOAD_NO_PROGRESS TRUE
)
FetchContent_MakeAvailable(get_googletest)

# GMock Issue: https://github.com/google/googletest/issues/2650
target_compile_options(gmock INTERFACE "-Wno-gnu-zero-variadic-macro-arguments")
