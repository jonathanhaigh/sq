/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_
#define SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_

#include "common_types/Primitive.h"
#include "util/typeutil.h"

#include <gsl/gsl>
#include <string_view>

namespace sq::test {

SQ_ND Primitive to_primitive(PrimitiveString &&v);
SQ_ND Primitive to_primitive(const PrimitiveString &v);
SQ_ND Primitive to_primitive(std::string_view v);
SQ_ND Primitive to_primitive(gsl::czstring<> v);
SQ_ND Primitive to_primitive(PrimitiveInt v);
SQ_ND Primitive to_primitive(int v);
SQ_ND Primitive to_primitive(PrimitiveFloat v);
SQ_ND Primitive to_primitive(PrimitiveBool v);

} // namespace sq::test

#endif // SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_
