/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqAnyPrimitiveImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqAnyPrimitiveImpl_h_

#include "core/typeutil.h"
#include "system/SqAnyPrimitive.gen.h"

namespace sq::system::linux {

class SqAnyPrimitiveImpl : public SqAnyPrimitive<SqAnyPrimitiveImpl> {
public:
  explicit SqAnyPrimitiveImpl(const Primitive &prim);

  SQ_ND Primitive to_primitive() const override;

private:
  Primitive prim_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqAnyPrimitiveImpl_h_
