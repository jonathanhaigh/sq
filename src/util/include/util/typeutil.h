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
#include <variant>

#define SQ_FWD(x) static_cast<decltype(x) &&>(x)

#define SQ_ND [[nodiscard]]
#define SQ_MU [[maybe_unused]]

namespace sq::util {

/**
 * Concept for types that can be dumped to a std::ostream.
 */
template <typename T> concept Printable = requires(T x, std::ostream &os) {
  os << x;
};

///@{
/**
 * Get whether the type T is a specialization of std::variant.
 */
template <typename T> struct IsVariant : std::false_type {};

template <typename... Types>
struct IsVariant<std::variant<Types...>> : std::true_type {};

template <typename T> inline constexpr bool is_variant_v = IsVariant<T>::value;
///@}

///@{
/**
 * Get whether the type T is one of the alternative types of the variant V.
 */
template <typename T, typename V>
requires is_variant_v<V> struct IsAlternative : std::false_type {};

template <typename T, typename... Types>
struct IsAlternative<T, std::variant<Types...>>
    : std::disjunction<std::is_same<T, Types>...> {};

template <typename T, typename V>
inline constexpr bool is_alternative_v = IsAlternative<T, V>::value;
///@}

/**
 * Concept for types that are alternatives of the variant V.
 */
template <typename T, typename V> concept Alternative = is_alternative_v<T, V>;

///@{
/**
 * Get whether T can be converted to one of the variant V's alternatives.
 */
template <typename T, typename V>
requires is_variant_v<V> struct IsConvertibleToAlternative : std::false_type {};

template <typename T, typename... Types>
struct IsConvertibleToAlternative<T, std::variant<Types...>>
    : std::disjunction<std::is_convertible<T, Types>...> {};

template <typename T, typename V>
inline constexpr bool is_convertible_to_alternative_v =
    IsConvertibleToAlternative<T, V>::value;
///@}

/**
 * Concept for types that are convertible to one of the variant V's
 * alternatives.
 */
template <typename T, typename V>
concept ConvertibleToAlternative = is_convertible_to_alternative_v<T, V>;

///@{
/**
 * A range whose distance can be obtained without invalidating the range.
 *
 * This is different to std::sized_range because it doesn't require:
 * - that size(rng) is available;
 * - that distance(rng) is a constant time operation.
 *
 * Essentially, it just rules out std::input_ranges without a known size.
 */
template <typename T>
concept SlowSizedRange =
    ranges::cpp20::forward_range<T> || ranges::cpp20::sized_range<T>;

/**
 * Convert an integral value to a gsl::index.
 *
 * Throws if the value can't be represented by a gsl::index.
 */
inline constexpr gsl::index to_index(std::integral auto v) {
  return gsl::narrow<gsl::index>(v);
}

/**
 * Convert an integral value to a std::size_t.
 *
 * Throws if the value can't be represented by a std::size_t.
 */
inline constexpr std::size_t to_size(std::integral auto v) {
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
template <typename T> inline constexpr bool enable_view<gsl::span<T, 0>> = true;

} // namespace ranges

#endif // SQ_INCLUDE_GUARD_util_typeutil_h_
