/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_

#include "system/SqString.gen.h"
#include "util/typeutil.h"

#include <string_view>

namespace sq::system::standard {

class SqStringImpl : public SqString<SqStringImpl> {
public:
  explicit SqStringImpl(std::string_view value);
  explicit SqStringImpl(PrimitiveString &&value);

  SQ_ND Primitive to_primitive() const override;

private:
  PrimitiveString value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_
