/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqGroupImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqGroupImpl_h_

#include "system/SqGroup.gen.h"
#include "util/typeutil.h"

#include <limits>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <vector>

namespace sq::system::linux {

class SqGroupImpl : public SqGroup<SqGroupImpl> {
public:
  explicit SqGroupImpl(gid_t gid);
  explicit SqGroupImpl(std::string_view name);

  SQ_ND Result get_gid() const;
  SQ_ND Result get_name() const;
  SQ_ND Result get_members() const;
  SQ_ND Primitive to_primitive() const override;

private:
  void ensure_fully_initialized() const;

  static constexpr auto invalid_gid_ = std::numeric_limits<gid_t>::max();

  mutable bool fully_initialized_ = false;
  mutable gid_t gid_ = invalid_gid_;
  mutable std::string name_;
  mutable std::vector<std::string> members_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqGroupImpl_h_
