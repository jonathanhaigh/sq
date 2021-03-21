/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqGroupImpl.h"

#include "core/ASSERT.h"
#include "system/linux/SqIntImpl.h"
#include "system/linux/SqStringImpl.h"
#include "system/linux/SqUserImpl.h"

#include <cerrno>
#include <fmt/format.h>
#include <grp.h>
#include <gsl/gsl>
#include <range/v3/view/transform.hpp>

namespace sq::system::linux {

SqGroupImpl::SqGroupImpl(gid_t gid) : gid_{gid} {}
SqGroupImpl::SqGroupImpl(std::string_view name) : name_{name} {}

Result SqGroupImpl::get_gid() const {
  if (gid_ == invalid_gid_) {
    ensure_fully_initialized();
  }
  return std::make_shared<SqIntImpl>(to_primitive_int(gid_, "GID"));
}

Result SqGroupImpl::get_name() const {
  if (name_.empty()) {
    ensure_fully_initialized();
  }
  return std::make_shared<SqStringImpl>(name_);
}

Result SqGroupImpl::get_members() const {
  if (members_.empty()) {
    ensure_fully_initialized();
  }
  return FieldRange<ranges::category::random_access | ranges::category::sized>{
      members_ | ranges::views::transform([](const auto &username) {
        return std::make_shared<SqUserImpl>(username);
      })};
}

Primitive SqGroupImpl::to_primitive() const {
  if (gid_ == invalid_gid_) {
    ensure_fully_initialized();
  }
  return to_primitive_int(gid_, "GID");
}

void SqGroupImpl::ensure_fully_initialized() const {
  Expects(gid_ != invalid_gid_ || !name_.empty());
  if (fully_initialized_) {
    return;
  }
  const auto have_gid = gid_ != invalid_gid_;

  errno = 0;
  const auto *const grp = have_gid ? getgrgid(gid_) : getgrnam(name_.c_str());
  if (grp == nullptr) {
    auto operation = have_gid
                         ? fmt::format("getgrgid for GID {}", gid_)
                         : fmt::format("getgrnam for group name {}", name_);
    if (errno == 0) {
      throw SystemError{fmt::format("{} failed: entry not found", operation)};
    }
    throw SystemError{operation, make_error_code(errno)};
  }

  if (have_gid) {
    ASSERT(grp->gr_gid == gid_);
    name_ = grp->gr_name;
  } else {
    ASSERT(grp->gr_name == name_);
    gid_ = grp->gr_gid;
  }
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  for (char **it = grp->gr_mem; *it != nullptr; ++it) {
    members_.emplace_back(*it);
  }
  fully_initialized_ = true;
}

} // namespace sq::system::linux
