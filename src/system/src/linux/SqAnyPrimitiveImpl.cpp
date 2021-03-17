/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqAnyPrimitiveImpl.h"

namespace sq::system::linux {

SqAnyPrimitiveImpl::SqAnyPrimitiveImpl(const Primitive &prim) : prim_{prim} {}
Primitive SqAnyPrimitiveImpl::to_primitive() const { return prim_; }

} // namespace sq::system::linux
