/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/ArgumentTypeError.h"

#include <sstream>

namespace sq {

namespace {

std::string create_error_message(const Primitive &received,
                                 std::string_view type_expected) {
  auto ss = std::ostringstream{};
  ss << "Invalid argument " << primitive_to_str(received) << " of type "
     << primitive_type_name(received) << " expecting " << type_expected;
  return ss.str();
}

} // namespace

ArgumentTypeError::ArgumentTypeError(const Primitive &received,
                                     std::string_view type_expected)
    : Exception(create_error_message(received, type_expected)) {}

} // namespace sq
