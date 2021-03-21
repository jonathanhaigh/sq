/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqIntImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqIntImpl_h_

#include "core/typeutil.h"
#include "system/SqInt.gen.h"

namespace sq::system::linux {

class SqIntImpl : public SqInt<SqIntImpl> {
public:
  explicit SqIntImpl(PrimitiveInt value);

  SQ_ND Primitive to_primitive() const override;

private:
  PrimitiveInt value_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqIntImpl_h_
