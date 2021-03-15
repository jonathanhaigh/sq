/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqFloatImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqFloatImpl_h_

#include "system/SqFloat.gen.h"
#include "util/typeutil.h"

namespace sq::system::linux {

class SqFloatImpl : public SqFloat<SqFloatImpl> {
public:
  explicit SqFloatImpl(PrimitiveFloat value);

  SQ_ND Primitive to_primitive() const override;

private:
  PrimitiveFloat value_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqFloatImpl_h_
