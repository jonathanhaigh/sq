/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqFloatImpl.h"

namespace sq::system::linux {

SqFloatImpl::SqFloatImpl(PrimitiveFloat value) : value_{value} {}

Primitive SqFloatImpl::to_primitive() const { return value_; }

} // namespace sq::system::linux
