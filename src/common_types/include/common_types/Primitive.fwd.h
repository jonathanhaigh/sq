/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_fwd_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_fwd_h_

#include <cstdint>
#include <string>
#include <variant>

namespace sq {

using PrimitiveString = std::string;
using PrimitiveInt = std::int64_t;
using PrimitiveFloat = double;
using PrimitiveBool = bool;
struct PrimitiveNull;

using Primitive = std::variant<PrimitiveString, PrimitiveInt, PrimitiveFloat,
                               PrimitiveBool, PrimitiveNull>;

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_fwd_h_
