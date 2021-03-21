/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_narrow_h_
#define SQ_INCLUDE_GUARD_core_narrow_h_

#include "core/typeutil.h"

#include <concepts>
#include <cstddef>
#include <gsl/gsl>
#include <string_view>

namespace sq {

/**
 * Wrapper around gsl::narrow that provides better diags on failure.
 *
 * @param value value to convert
 * @param format_args optional arguments, passed to fmt::format that describe
 *        what kind of thing is being converted.
 *
 * E.g.
 *    narrow<int>(0.5, "number of words in file {}", path)
 * might throw an exception with a message like:
 *    number of words in file "/some/file" (0.5) does not fit in type int
 *
 * If the optional format_args args aren't given then the value's type is used
 * as a description instead. E.g.
 *    narrow<int>(0.5)
 * might throw an exception with a message like:
 *    float (0.5) does not fit in type int
 */
template <typename T, typename U, typename... FormatArgs>
SQ_ND constexpr T narrow(U value, FormatArgs &&...format_args);

/**
 * Convert an integral value to a gsl::index.
 *
 * Throws if the value can't be represented by a gsl::index.
 */
SQ_ND constexpr gsl::index to_index(auto value, auto &&...format_args);

/**
 * Convert an integral value to a std::size_t.
 *
 * Throws if the value can't be represented by a std::size_t.
 */
SQ_ND constexpr std::size_t to_size(auto value, auto &&...format_args);

} // namespace sq

#include "core/narrow.inl.h"

#endif // SQ_INCLUDE_GUARD_core_narrow_h_
