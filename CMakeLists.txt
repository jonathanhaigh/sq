cmake_minimum_required(VERSION 3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_subdirectory(3rdparty)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()
include(GoogleTest)

project(Sq
    VERSION 0.0.1
    DESCRIPTION "Command line tool to obtain system information in JSON"
    LANGUAGES CXX
)

if("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

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

option(FORCE_COLOR "Force coloured compiler output" TRUE)
if("${FORCE_COLOR}" OR "$ENV{CLICOLOR_FORCE}")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL GNU)
        add_compile_options(-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL Clang)
        add_compile_options(-fcolor-diagnostics)
    endif()
endif()

add_subdirectory(src)
