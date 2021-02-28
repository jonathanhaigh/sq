/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_
#define SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_

#include "common_types/Primitive.h"

#include <gsl/gsl>
#include <string_view>

namespace sq::test {

[[nodiscard]] Primitive to_primitive(PrimitiveString&& v);
[[nodiscard]] Primitive to_primitive(const PrimitiveString& v);
[[nodiscard]] Primitive to_primitive(std::string_view v);
[[nodiscard]] Primitive to_primitive(gsl::czstring<> v);
[[nodiscard]] Primitive to_primitive(PrimitiveInt v);
[[nodiscard]] Primitive to_primitive(int v);
[[nodiscard]] Primitive to_primitive(PrimitiveFloat v);
[[nodiscard]] Primitive to_primitive(PrimitiveBool v);

} // namespace sq::test

#endif // SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_
