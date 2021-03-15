/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqBoolImpl.h"

namespace sq::system::linux {

SqBoolImpl::SqBoolImpl(PrimitiveBool value) : value_{value} {}

Primitive SqBoolImpl::to_primitive() const { return value_; }

} // namespace sq::system::linux
