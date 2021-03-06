/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqStringImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqStringImpl_h_

#include "core/typeutil.h"
#include "system/SqString.gen.h"

#include <string_view>

namespace sq::system::linux {

class SqStringImpl : public SqString<SqStringImpl> {
public:
  explicit SqStringImpl(std::string_view value);

  SQ_ND Primitive to_primitive() const override;

private:
  PrimitiveString value_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqStringImpl_h_
