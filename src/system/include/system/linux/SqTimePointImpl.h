/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqTimePointImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqTimePointImpl_h_

#include "core/typeutil.h"
#include "system/SqTimePoint.gen.h"

#include <chrono>
#include <ctime>

namespace sq::system::linux {

class SqTimePointImpl : public SqTimePoint<SqTimePointImpl> {
public:
  using Clock = std::chrono::system_clock;
  using TimePoint = Clock::time_point;

  explicit SqTimePointImpl(TimePoint tp);

  static std::shared_ptr<SqTimePointImpl> from_unix_timespec(std::timespec ts);

  SQ_ND Primitive to_primitive() const override;

private:
  TimePoint tp_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqTimePointImpl_h_
