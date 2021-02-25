/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_util_typeutil_h_
#define SQ_INCLUDE_GUARD_util_typeutil_h_

#include <concepts>
#include <cstddef>
#include <gsl/gsl>
#include <type_traits>

namespace sq::util {

template <typename T>
using disable_lvalues_t = std::enable_if_t<!std::is_lvalue_reference_v<T>>;

template <std::integral T>
inline constexpr std::ptrdiff_t to_ptrdiff_t(T v)
{
    return gsl::narrow<std::ptrdiff_t>(v);
}

template <std::integral T>
inline constexpr std::size_t to_size_t(T v)
{
    return gsl::narrow<std::size_t>(v);
}

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_typeutil_h_
