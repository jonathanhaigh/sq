# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

function(sq_expand_for_each_type FORMAT_STRING OUTPUT_VAR)
    set(SQ_TYPES
        SqAnyPrimitive
        SqBool
        SqDataSize
        SqDevice
        SqFieldSchema
        SqFile
        SqFileMode
        SqFloat
        SqInt
        SqParamSchema
        SqPath
        SqPrimitiveTypeSchema
        SqRoot
        SqSchema
        SqString
        SqTimePoint
        SqTypeSchema
    )
    set(EXPANDED_STRINGS)
    foreach(SQ_TYPE ${SQ_TYPES})
        string(REPLACE "%s" ${SQ_TYPE} EXPANDED_STRING ${FORMAT_STRING})
        list(APPEND EXPANDED_STRINGS ${EXPANDED_STRING})
    endforeach()
    set(${OUTPUT_VAR} ${EXPANDED_STRINGS} PARENT_SCOPE)
endfunction()

function(sq_generate_file_for_each_type TEMPLATE_NAME OUTPUT_PATH_FORMAT OUTPUT_PATHS_VAR)
    sq_expand_for_each_type(${OUTPUT_PATH_FORMAT} GENERATED_FILES)

    set(OUTPUT_DIRS)
    foreach(GENERATED_FILE ${GENERATED_FILES})
        get_filename_component(OUTPUT_DIR "${GENERATED_FILE}" DIRECTORY)
        list(APPEND OUTPUT_DIRS "${OUTPUT_DIR}")
    endforeach()

    get_target_property(LUA_EXE lua LOCATION)
    add_custom_command(
        OUTPUT ${GENERATED_FILES}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scripts"
        COMMAND ${CMAKE_COMMAND} -E make_directory
            ${OUTPUT_DIRS}
        COMMAND "${LUA_EXE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_file_for_each_type.lua"
            "${CMAKE_CURRENT_SOURCE_DIR}/schema.json"
            "${CMAKE_CURRENT_SOURCE_DIR}/templates/${TEMPLATE_NAME}.template"
            "${OUTPUT_PATH_FORMAT}"
        DEPENDS
            lua
            lua_lunajson
            lua_liluat
            "scripts/sq_schema_util.lua"
            "scripts/generate_file_for_each_type.lua"
            "schema.json"
            "templates/${TEMPLATE_NAME}.template"
        VERBATIM
    )
    set(${OUTPUT_PATHS_VAR} ${GENERATED_FILES} PARENT_SCOPE)
endfunction()

function(sq_generate_file TEMPLATE_NAME OUTPUT_PATH)

    get_filename_component(OUTPUT_DIR "${OUTPUT_PATH}" DIRECTORY)

    get_target_property(LUA_EXE lua LOCATION)
    add_custom_command(
        OUTPUT "${OUTPUT_PATH}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scripts"
        COMMAND ${CMAKE_COMMAND} -E make_directory
            ${OUTPUT_DIR}
        COMMAND "${LUA_EXE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_file.lua"
            "${CMAKE_CURRENT_SOURCE_DIR}/schema.json"
            "${CMAKE_CURRENT_SOURCE_DIR}/templates/${TEMPLATE_NAME}.template"
            "${OUTPUT_PATH}"
        DEPENDS
            lua
            lua_lunajson
            lua_liluat
            "scripts/sq_schema_util.lua"
            "scripts/generate_file.lua"
            "schema.json"
            "templates/${TEMPLATE_NAME}.template"
        VERBATIM
    )
    set(${OUTPUT_PATHS_VAR} ${GENERATED_FILES} PARENT_SCOPE)
endfunction()
