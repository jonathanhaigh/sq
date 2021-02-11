cmake_minimum_required(VERSION 3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

add_subdirectory(3rdparty)

project(Sq
    VERSION 0.0.1
    DESCRIPTION "Command line tool to obtain system information in JSON"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=*,-google-runtime-references,-hicpp-no-array-decay,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-fuchsia-overloaded-operator,-modernize-use-trailing-return-type,-fuchsia-default-arguments-calls,-fuchsia-trailing-return,-modernize-pass-by-value)

set(SQ_GCC_STYLE_WARNING_FLAGS
    -Wall
    -Wextra
    -pedantic-errors
    -pedantic
    -Wconversion
    -Wsign-conversion
    -Werror
    -Wold-style-cast
    -Woverloaded-virtual
)

set(SQ_MSVC_WARNING_FLAGS
    /W4
    /WX
)

# Compilers (as they would appear in CMAKE_CXX_COMPILER_ID) that take GCC style
# flags.
set(SQ_GCC_STYLE_COMPILERS
    GNU
    Clang
    AppleClang
)

if ("${CMAKE_CXX_COMPILER_ID}" IN_LIST SQ_GCC_STYLE_COMPILERS)
    set(SQ_WARNING_FLAGS ${SQ_GCC_STYLE_WARNING_FLAGS})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL MSVC)
    set(SQ_WARNING_FLAGS ${SQ_MSVC_WARNING_FLAGS})
endif()

add_compile_options(${SQ_WARNING_FLAGS})

add_subdirectory(src)
