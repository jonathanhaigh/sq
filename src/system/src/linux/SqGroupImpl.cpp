/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqGroupImpl.h"

#include "system/linux/SqIntImpl.h"

#include <fmt/format.h>
#include <gsl/gsl>

namespace sq::system::linux {

SqGroupImpl::SqGroupImpl(gid_t gid) : gid_{gid} {}

Result SqGroupImpl::get_gid() const {
  try {
    return std::make_shared<SqIntImpl>(gsl::narrow<PrimitiveInt>(gid_));
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{
        fmt::format("cannot fit GID {} in type PrimitiveInt", gid_)};
  }
}

Primitive SqGroupImpl::to_primitive() const {
  try {
    return gsl::narrow<PrimitiveInt>(gid_);
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{
        fmt::format("cannot fit GID {} in type PrimitiveInt", gid_)};
  }
}

} // namespace sq::system::linux
