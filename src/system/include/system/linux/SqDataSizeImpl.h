/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqDataSizeImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqDataSizeImpl_h_

#include "system/SqDataSize.gen.h"
#include "util/typeutil.h"

#include <cstdint>

namespace sq::system::linux {

class SqDataSizeImpl : public SqDataSize<SqDataSizeImpl> {
public:
  explicit SqDataSizeImpl(PrimitiveInt value);

  SQ_ND Result get_B() const;
  SQ_ND Result get_KiB() const;
  SQ_ND Result get_kB() const;
  SQ_ND Result get_MiB() const;
  SQ_ND Result get_MB() const;
  SQ_ND Result get_GiB() const;
  SQ_ND Result get_GB() const;
  SQ_ND Result get_TiB() const;
  SQ_ND Result get_TB() const;
  SQ_ND Result get_PiB() const;
  SQ_ND Result get_PB() const;
  SQ_ND Result get_EiB() const;
  SQ_ND Result get_EB() const;
  SQ_ND Primitive to_primitive() const override;

private:
  PrimitiveInt value_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqDataSizeImpl_h_
