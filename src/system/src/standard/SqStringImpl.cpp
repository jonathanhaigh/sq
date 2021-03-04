/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqStringImpl.h"

namespace sq::system::standard {

SqStringImpl::SqStringImpl(std::string_view value) : value_{value} {}

SqStringImpl::SqStringImpl(PrimitiveString &&value)
    : value_{std::move(value)} {}

Primitive SqStringImpl::to_primitive() const { return value_; }

} // namespace sq::system::standard
