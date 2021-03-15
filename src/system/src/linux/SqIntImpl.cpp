/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqIntImpl.h"

namespace sq::system::linux {

SqIntImpl::SqIntImpl(PrimitiveInt value) : value_{value} {}

Primitive SqIntImpl::to_primitive() const { return value_; }

} // namespace sq::system::linux
