# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

set(SQ_ALL_LIBS
    common_types
    parser
    results
    serialization
    system
    util
)

add_executable(sq main.cpp)
target_link_libraries(sq gsl)

foreach(SQ_LIB ${SQ_ALL_LIBS})
    add_subdirectory("${SQ_LIB}")
    target_link_libraries(sq "sq_${SQ_LIB}")

    if (SQ_BUILD_TESTS)
        add_subdirectory("${SQ_LIB}/test")
    endif()
endforeach()

install(TARGETS sq RUNTIME)
