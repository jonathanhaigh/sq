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

    add_subdirectory("${SQ_LIB}/test")
endforeach()
