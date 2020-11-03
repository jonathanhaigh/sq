add_subdirectory(schema)
add_subdirectory(parser)
add_subdirectory(util)
add_subdirectory(ast)
add_subdirectory(results)
add_subdirectory(serialization)

set(SQ_SRC
    main.cpp
)

add_executable(sq ${SQ_SRC})
target_link_libraries(sq rapidjson)
target_link_libraries(sq sq_schema)
target_link_libraries(sq sq_ast)
target_link_libraries(sq sq_results)
target_link_libraries(sq sq_serialization)
