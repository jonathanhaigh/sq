/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/InvalidFieldError.h"

#include <sstream>

namespace sq {

namespace {

std::string make_invalid_field_error_message(
    std::string_view sq_type,
    std::string_view field
)
{
    auto os = std::ostringstream{};
    os << sq_type << " has no field \"" << field << "\"";
    return os.str();
}

} // namespace

InvalidFieldError::InvalidFieldError(
    std::string_view sq_type,
    std::string_view field
)
    : Exception{make_invalid_field_error_message(sq_type, field)}
{}

} // namespace sq
