/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqUserImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqUserImpl_h_

#include "system/SqUser.gen.h"
#include "util/typeutil.h"

#include <string>
#include <string_view>
#include <sys/types.h>

namespace sq::system::linux {

class SqUserImpl : public SqUser<SqUserImpl> {
public:
  explicit SqUserImpl(uid_t uid);
  explicit SqUserImpl(std::string_view username);

  SQ_ND Result get_uid() const;
  SQ_ND Result get_username() const;
  SQ_ND Result get_group() const;
  SQ_ND Result get_name() const;
  SQ_ND Result get_gecos() const;
  SQ_ND Result get_home() const;
  SQ_ND Result get_shell() const;
  SQ_ND Primitive to_primitive() const override;

private:
  void ensure_fully_initialized() const;

  static constexpr uid_t invalid_uid_ = std::numeric_limits<uid_t>::max();
  static constexpr gid_t invalid_gid_ = std::numeric_limits<gid_t>::max();

  mutable bool fully_initialized_ = false;
  mutable uid_t uid_ = invalid_uid_;
  mutable std::string username_;
  mutable gid_t gid_ = invalid_gid_;
  mutable std::string gecos_;
  mutable std::string home_;
  mutable std::string shell_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqUserImpl_h_
