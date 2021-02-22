find_library(ANTLR4_RUNTIME_PATH
    "antlr4-runtime"
)
find_path(ANTLR4_RUNTIME_INCLUDE_PATH
    NAMES "antlr4-runtime.h"
    PATH_SUFFIXES "antlr4-runtime"
)

if (ANTLR4_RUNTIME_PATH AND ANTLR4_RUNTIME_INCLUDE_PATH)

    add_library(antlr4_runtime UNKNOWN IMPORTED GLOBAL)
    set_target_properties(antlr4_runtime PROPERTIES
        IMPORTED_LOCATION "${ANTLR4_RUNTIME_PATH}"
    )
    target_include_directories(antlr4_runtime INTERFACE "${ANTLR4_RUNTIME_INCLUDE_PATH}")

else()
    include(ExternalProject)

    # NOTE:
    # The ANTLR4 runtime header files contain checks for the version of C++ being
    # used, and, when the C++ standard is >= 17, extra overloads of functions are
    # enabled (e.g. an ANTLRInputStream constructor that takes a std::string_view
    # param).
    #
    # It's important that the ANTLR4 runtime library that SQ links with contains a
    # superset of the functions provided by the header files that are included by
    # the SQ build. Otherwise the SQ build will try to use functions that don't
    # exist in the runtime library.
    #
    # The main SQ build uses C++ >= 17 so we need to build the ANTLR4 runtime
    # library with at least C++17.
    #
    # As of 2021-02-21 there isn't a proper release of the ANTLR4 C++ runtime that
    # lets us easily build for C++ >= 17, so a specific non-tagged commit is used
    # below where the ANTLR4 build will respect the CMAKE_CXX_STANDARD value given
    # on the CMake command line.
    # 
    set(ANTLR4_RUNTIME_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/antlr4-runtime")
    ExternalProject_Add(antlr4_runtime_project
        URL https://github.com/antlr/antlr4/tarball/98a3f7d6408f35641141ed445579d0a63e53bc61
        SOURCE_SUBDIR "runtime/Cpp"
        INSTALL_DIR "${ANTLR4_RUNTIME_INSTALL_DIR}"
        CMAKE_ARGS
            "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
            "-DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}"
            "-DCMAKE_INSTALL_PREFIX=${ANTLR4_RUNTIME_INSTALL_DIR}"
            "-DANTLR4_INSTALL=TRUE"
    )
    add_library(antlr4_runtime STATIC IMPORTED GLOBAL)
    add_dependencies(antlr4_runtime antlr4_runtime_project)
    set_target_properties(antlr4_runtime
        PROPERTIES
        IMPORTED_LOCATION "${ANTLR4_RUNTIME_INSTALL_DIR}/lib/antlr4-runtime.a"
    )

    # The antlr4-runtime include directory is created below at configure time.
    # Nothing actually needs to use the directory at configure time, but CMake
    # will complain if it doesn't exist when we call
    # target_include_directories.
    file(MAKE_DIRECTORY "${ANTLR4_RUNTIME_INSTALL_DIR}/include/antlr4-runtime")
    target_include_directories(antlr4_runtime
        INTERFACE "${ANTLR4_RUNTIME_INSTALL_DIR}/include/antlr4-runtime"
    )
endif()
