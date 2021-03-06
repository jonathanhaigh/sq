# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

find_package(Python3 COMPONENTS Interpreter)
message("Python3 interpreter: ${Python3_EXECUTABLE}")

execute_process(
    COMMAND "${Python3_EXECUTABLE}" -m pytest --version
    OUTPUT_QUIET
    ERROR_VARIABLE SQ_PYTEST_ERROR
    RESULT_VARIABLE SQ_PYTEST_RESULT)
if(NOT "${SQ_PYTEST_RESULT}" EQUAL 0)
    message(FATAL_ERROR "Failed to get pytest version: ${SQ_PYTEST_ERROR}")
endif()

add_test(
    NAME integration_tests
    COMMAND cmake -E env
        "SQ_BINARY=$<TARGET_FILE:sq>"
        python3 -m pytest -p no:cacheprovider -v
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
