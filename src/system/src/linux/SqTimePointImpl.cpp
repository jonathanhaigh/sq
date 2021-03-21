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
  return to_primitive_int(
      std::chrono::duration_cast<std::chrono::seconds>(tp_.time_since_epoch())
          .count(),
      "Time since epoch in seconds");
}

} // namespace sq::system::linux
