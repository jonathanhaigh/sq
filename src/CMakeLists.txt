set(SQ_ALL_LIBS
    ast
    common_types
    parser
    results
    serialization
    system
    util
)

foreach(SQ_LIB ${SQ_ALL_LIBS})
    add_subdirectory("${SQ_LIB}")
endforeach()

add_executable(sq main.cpp)
target_link_libraries(sq gsl)

foreach(SQ_LIB ${SQ_ALL_LIBS})
    target_link_libraries(sq "sq_${SQ_LIB}")
endforeach()

add_executable(sq-test)
target_link_libraries(sq-test gtest_main)
target_link_libraries(sq-test gtest)
set_target_properties(sq-test PROPERTIES CXX_CLANG_TIDY "")

foreach(SQ_LIB ${SQ_ALL_LIBS})
    target_link_libraries(sq-test "sq_${SQ_LIB}")
    target_sources(sq-test PRIVATE "${SQ_LIB}/test/test_${SQ_LIB}.cpp")
endforeach()
