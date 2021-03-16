# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

# If the variable for an option is already set when the option() command is
# used, use the value in the existing variable rather than resetting it.
# This is required so that when the INSTALL_GTEST variable is set to OFF,
# GoogleTest actually honours the value OFF rather than using its default
# value of ON.
#
# We can't use e.g. policy(SET CMP0077 NEW) because the GoogleTest cmake file's
# cmake_minimum_required() command would undo that in GoogleTest's
# subdirectory. See https://gitlab.kitware.com/cmake/cmake/-/issues/20312 for
# details.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(Sq
    VERSION 0.0.1
    DESCRIPTION "Command line tool to obtain system information in JSON"
    LANGUAGES CXX
)

# Set the C++ standard before adding the 3rdparty subdir so that everything
# gets compiled for the same standard
include(cpp_standard.cmake)

# The clang/gcc sanitizers tend to work better (or at all) when dependencies
# are also instrumented so set up the sanitizer flags before adding the
# 3rdparty subdir.
include(sanitizer_flags.cmake)

# Include before 3rd party so that GoogleTest classes are instrumented for
# coverage
include(coverage_flags.cmake)

option(SQ_BUILD_TESTS "Build SQ's tests" FALSE)

# We add the gtest repo with the equivalent of "add_subdirectory" so by default
# gtest would be installed by "cmake --build . --target install". We don't want
# that.
set(INSTALL_GTEST OFF)

add_subdirectory(3rdparty)

# For CodeChecker
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (SQ_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
endif()

include(warning_flags.cmake)
include(optimization_flags.cmake)
include(options.cmake)
include(clang-tidy.cmake)

add_subdirectory(src)
if (SQ_BUILD_TESTS)
    add_subdirectory(test)
endif()
