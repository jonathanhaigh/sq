/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_narrow_inl_h_
#define SQ_INCLUDE_GUARD_core_narrow_inl_h_

#include "core/errors.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gsl/gsl>

namespace sq {

template <typename T, typename U, typename... FormatArgs>
constexpr T narrow(U value, FormatArgs &&...format_args) {
  try {
    return gsl::narrow<T>(value);
  } catch (gsl::narrowing_error &e) {
    throw NarrowingError{T{}, value, SQ_FWD(format_args)...};
  }
}

constexpr gsl::index to_index(auto value, auto &&...format_args) {
  return narrow<gsl::index>(SQ_FWD(value), SQ_FWD(format_args)...);
}

constexpr std::size_t to_size(auto value, auto &&...format_args) {
  return narrow<std::size_t>(SQ_FWD(value), SQ_FWD(format_args)...);
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_core_narrow_inl_h_
