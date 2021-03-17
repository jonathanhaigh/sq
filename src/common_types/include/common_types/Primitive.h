/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_h_

#include "Primitive.fwd.h"

#include "util/typeutil.h"

#include <type_traits>

namespace sq {

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

template <PrimitiveAlternative T>
SQ_ND T convert_primitive(const Primitive &value);

struct PrimitiveNull {
  auto operator<=>(const PrimitiveNull &) const noexcept = default;
  bool operator==(const PrimitiveNull &) const noexcept = default;
};

std::ostream &operator<<(std::ostream &os, const PrimitiveNull &pn);

inline constexpr auto primitive_null = PrimitiveNull{};

} // namespace sq

#include "Primitive.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_h_
