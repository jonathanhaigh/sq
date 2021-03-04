/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "test/Primitive_test_util.h"

namespace sq::test {

Primitive to_primitive(const PrimitiveString &v) { return v; }
Primitive to_primitive(PrimitiveString &&v) { return std::move(v); }
Primitive to_primitive(gsl::czstring<> v) { return PrimitiveString{v}; }
Primitive to_primitive(std::string_view v) { return PrimitiveString{v}; }
Primitive to_primitive(PrimitiveInt v) { return v; }
Primitive to_primitive(int v) { return PrimitiveInt{v}; }
Primitive to_primitive(PrimitiveFloat v) { return v; }
Primitive to_primitive(PrimitiveBool v) { return v; }

} // namespace sq::test
