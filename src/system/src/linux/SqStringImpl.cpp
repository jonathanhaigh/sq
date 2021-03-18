/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqStringImpl.h"

namespace sq::system::linux {

SqStringImpl::SqStringImpl(std::string_view value) : value_{value} {}

Primitive SqStringImpl::to_primitive() const { return value_; }

} // namespace sq::system::linux
