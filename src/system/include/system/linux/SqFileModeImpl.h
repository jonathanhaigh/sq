/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqFileModeImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqFileModeImpl_h_

#include "system/SqFileMode.gen.h"
#include "util/typeutil.h"

#include <concepts>
#include <sys/types.h>

namespace sq::system::linux {

class SqFileModeImpl : public SqFileMode<SqFileModeImpl> {
public:
  explicit SqFileModeImpl(mode_t value);

  SQ_ND Result get_permissions() const;
  SQ_ND Result get_suid() const;
  SQ_ND Result get_sgid() const;
  SQ_ND Result get_sticky() const;

  SQ_ND Primitive to_primitive() const override;

private:
  mode_t value_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqFileModeImpl_h_
