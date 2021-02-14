set(SQ_ALL_LIBS
    ast
    common_types
    parser
    results
    serialization
    system
    util
)

add_executable(sq main.cpp)
target_link_libraries(sq gsl)

add_executable(sq-test test.cpp)
target_link_libraries(sq-test gtest_main)

foreach(SQ_LIB ${SQ_ALL_LIBS})
    add_subdirectory("${SQ_LIB}")
    target_link_libraries(sq "sq_${SQ_LIB}")

    add_subdirectory("${SQ_LIB}/test")
    target_link_libraries(sq-test "sq_${SQ_LIB}_test")
endforeach()
