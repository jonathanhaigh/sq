/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_h_

#include "util/typeutil.h"

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

template <util::Alternative<Primitive> P>
struct PrimitiveTypeName;

template <util::Alternative<Primitive> P>
inline constexpr std::string_view primitive_type_name_v = PrimitiveTypeName<P>::value;

[[nodiscard]] std::string_view primitive_type_name(const Primitive& value);

[[nodiscard]] std::string primitive_to_str(const Primitive& value);

template <util::Alternative<Primitive> T>
[[nodiscard]] std::string primitive_to_str(const T& value);

} // namespace sq

#include "Primitive.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_h_
