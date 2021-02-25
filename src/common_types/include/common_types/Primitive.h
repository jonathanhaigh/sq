/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_h_

#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>

namespace sq {

using PrimitiveString = std::string;
using PrimitiveInt = std::int64_t;
using PrimitiveFloat = double;
using PrimitiveBool = bool;

using Primitive = std::variant<
    PrimitiveString,
    PrimitiveInt,
    PrimitiveFloat,
    PrimitiveBool
>;

namespace detail {

template <typename T>
struct PrimitiveTraits;

} // namespace detail

template <typename T>
inline constexpr bool is_sq_primitive_v = detail::PrimitiveTraits<T>::is_primitive;

template <typename T>
inline constexpr const char* primitive_type_name_v = detail::PrimitiveTraits<T>::name;

[[nodiscard]] const char* primitive_type_name(const Primitive& value);

[[nodiscard]] std::string primitive_to_str(const Primitive& value);

template <typename T, typename = std::enable_if_t<is_sq_primitive_v<T>>>
[[nodiscard]] std::string primitive_to_str(const T& value);

} // namespace sq

#include "Primitive.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_h_
