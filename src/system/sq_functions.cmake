function(sq_expand_for_each_type FORMAT_STRING OUTPUT_VAR)
    set(SQ_TYPES
        SqBool
        SqDataSize
        SqFloat
        SqInt
        SqPath
        SqRoot
        SqString
    )
    set(EXPANDED_STRINGS)
    foreach(SQ_TYPE ${SQ_TYPES})
        string(REPLACE "%s" ${SQ_TYPE} EXPANDED_STRING ${FORMAT_STRING})
        list(APPEND EXPANDED_STRINGS ${EXPANDED_STRING})
    endforeach()
    set(${OUTPUT_VAR} ${EXPANDED_STRINGS} PARENT_SCOPE)
endfunction()

function(sq_generate_system_files TEMPLATE_NAME OUTPUT_PATH_FORMAT OUTPUT_PATHS_VAR)
    sq_expand_for_each_type(${OUTPUT_PATH_FORMAT} GENERATED_FILES)
    add_custom_command(
        OUTPUT ${GENERATED_FILES}
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/generate_system_files.py"
            "${TEMPLATE_NAME}"
            "${OUTPUT_PATH_FORMAT}"
        DEPENDS
            "schema.json"
            "generate_system_files.py"
            "templates/${TEMPLATE_NAME}.jinja"
        VERBATIM
    )
    set(${OUTPUT_PATHS_VAR} ${GENERATED_FILES} PARENT_SCOPE)
endfunction()