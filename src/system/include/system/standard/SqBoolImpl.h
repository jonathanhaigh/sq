/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_

#include "system/SqBool.gen.h"
#include "util/typeutil.h"

namespace sq::system::standard {

class SqBoolImpl : public SqBool<SqBoolImpl> {
public:
  explicit SqBoolImpl(PrimitiveBool value);

  SQ_ND Primitive to_primitive() const override;

private:
  PrimitiveBool value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_
