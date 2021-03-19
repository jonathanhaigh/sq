/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqUserImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqUserImpl_h_

#include "system/SqUser.gen.h"
#include "util/typeutil.h"

#include <sys/types.h>

namespace sq::system::linux {

class SqUserImpl : public SqUser<SqUserImpl> {
public:
  explicit SqUserImpl(uid_t uid);

  SQ_ND Result get_uid() const;
  SQ_ND Primitive to_primitive() const override;

private:
  uid_t uid_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqUserImpl_h_
