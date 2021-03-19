/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqUserImpl.h"

#include "system/linux/SqIntImpl.h"

#include <fmt/format.h>
#include <gsl/gsl>

namespace sq::system::linux {

SqUserImpl::SqUserImpl(uid_t uid) : uid_{uid} {}

Result SqUserImpl::get_uid() const {
  try {
    return std::make_shared<SqIntImpl>(gsl::narrow<PrimitiveInt>(uid_));
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{
        fmt::format("cannot fit UID {} in type PrimitiveInt", uid_)};
  }
}

Primitive SqUserImpl::to_primitive() const {
  try {
    return gsl::narrow<PrimitiveInt>(uid_);
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{
        fmt::format("cannot fit UID {} in type PrimitiveInt", uid_)};
  }
}

} // namespace sq::system::linux
