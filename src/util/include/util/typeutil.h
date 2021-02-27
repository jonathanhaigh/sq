/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_util_typeutil_h_
#define SQ_INCLUDE_GUARD_util_typeutil_h_

#include <concepts>
#include <cstddef>
#include <gsl/gsl>
#include <range/v3/range/concepts.hpp>
#include <type_traits>

namespace sq::util {

template <typename T>
using disable_lvalues_t = std::enable_if_t<!std::is_lvalue_reference_v<T>>;

template <std::integral T>
inline constexpr gsl::index to_index(T v)
{
    return gsl::narrow<gsl::index>(v);
}

template <std::integral T>
inline constexpr std::size_t to_size(T v)
{
    return gsl::narrow<std::size_t>(v);
}

} // namespace sq::util

namespace ranges {

// Tell the ranges library that gsl::spans with dynamic extent are views. Note
// that the view concept requires a default constructor so sized gsl::spans are
// *not* views...
template <typename T>
inline constexpr bool enable_view<gsl::span<T, gsl::dynamic_extent>> = true;

// ... unless the size is zero.
template <typename T>
inline constexpr bool enable_view<gsl::span<T, 0>> = true;

} // namespace ranges

#endif // SQ_INCLUDE_GUARD_util_typeutil_h_
