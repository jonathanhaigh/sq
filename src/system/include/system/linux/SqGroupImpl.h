/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqGroupImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqGroupImpl_h_

#include "system/SqGroup.gen.h"
#include "util/typeutil.h"

#include <sys/types.h>

namespace sq::system::linux {

class SqGroupImpl : public SqGroup<SqGroupImpl> {
public:
  explicit SqGroupImpl(gid_t gid);

  SQ_ND Result get_gid() const;
  SQ_ND Primitive to_primitive() const override;

private:
  gid_t gid_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqGroupImpl_h_
