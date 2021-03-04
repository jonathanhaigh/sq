/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqFloatImpl.h"

namespace sq::system::standard {

SqFloatImpl::SqFloatImpl(PrimitiveFloat value) : value_{value} {}

Primitive SqFloatImpl::to_primitive() const { return value_; }

} // namespace sq::system::standard
