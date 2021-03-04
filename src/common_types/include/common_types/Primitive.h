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

using Primitive =
    std::variant<PrimitiveString, PrimitiveInt, PrimitiveFloat, PrimitiveBool>;

template <typename T>
concept PrimitiveAlternative = util::Alternative<T, Primitive>;

template <typename T>
concept PrimitiveLike = util::ConvertibleToAlternative<T, Primitive>;

template <PrimitiveAlternative P> struct PrimitiveTypeName;

template <PrimitiveAlternative P>
inline constexpr std::string_view primitive_type_name_v =
    PrimitiveTypeName<P>::value;

SQ_ND std::string_view primitive_type_name(const Primitive &value);
SQ_ND std::string primitive_to_str(const Primitive &value);
SQ_ND std::string primitive_to_str(const PrimitiveAlternative auto &value);

} // namespace sq

#include "Primitive.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_h_
