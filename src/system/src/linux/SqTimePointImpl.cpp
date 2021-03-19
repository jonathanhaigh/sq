/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqTimePointImpl.h"

namespace sq::system::linux {

SqTimePointImpl::SqTimePointImpl(TimePoint tp) : tp_{tp} {}

std::shared_ptr<SqTimePointImpl>
SqTimePointImpl::from_unix_timespec(std::timespec ts) {
  return std::make_shared<SqTimePointImpl>(TimePoint{
      std::chrono::seconds{ts.tv_sec} + std::chrono::nanoseconds{ts.tv_nsec}});
}

Primitive SqTimePointImpl::to_primitive() const {
  return gsl::narrow<PrimitiveInt>(
      std::chrono::duration_cast<std::chrono::seconds>(tp_.time_since_epoch())
          .count());
}

} // namespace sq::system::linux
