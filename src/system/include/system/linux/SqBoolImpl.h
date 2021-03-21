/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqBoolImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqBoolImpl_h_

#include "core/typeutil.h"
#include "system/SqBool.gen.h"

namespace sq::system::linux {

class SqBoolImpl : public SqBool<SqBoolImpl> {
public:
  explicit SqBoolImpl(PrimitiveBool value);

  SQ_ND Primitive to_primitive() const override;

private:
  PrimitiveBool value_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqBoolImpl_h_
