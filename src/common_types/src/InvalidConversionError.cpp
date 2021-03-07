/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/InvalidConversionError.h"

#include <sstream>

namespace sq {

namespace {

std::string make_error_message(std::string_view from, std::string_view to) {
  auto ss = std::ostringstream{};
  ss << "cannot convert " << from << " to " << to;
  return ss.str();
}

} // namespace

InvalidConversionError::InvalidConversionError(std::string_view from,
                                               std::string_view to)
    : InvalidConversionError{make_error_message(from, to)} {}

} // namespace sq
