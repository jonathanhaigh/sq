/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/ArgumentMissingError.h"

#include <sstream>

namespace sq {

namespace {

std::string create_error_message(
    std::string_view arg_name,
    std::string_view arg_type
)
{
    auto ss = std::ostringstream{};
    ss << "Missing argument " << arg_name
       << " of type " << arg_type;
    return ss.str();
}

} // namespace

ArgumentMissingError::ArgumentMissingError(
    std::string_view arg_name,
    std::string_view arg_type
)
    : Exception(create_error_message(arg_name, arg_type))
{ }

} // namespace sq

