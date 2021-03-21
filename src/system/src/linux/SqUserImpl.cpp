/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqUserImpl.h"

#include "common_types/errors.h"
#include "system/linux/SqGroupImpl.h"
#include "system/linux/SqIntImpl.h"
#include "system/linux/SqPathImpl.h"
#include "system/linux/SqStringImpl.h"
#include "util/ASSERT.h"
#include "util/typeutil.h"

#include <cerrno>
#include <fmt/format.h>
#include <gsl/gsl>
#include <pwd.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <sys/types.h>
#include <unistd.h>

namespace sq::system::linux {

SqUserImpl::SqUserImpl(uid_t uid) : uid_{uid} {}
SqUserImpl::SqUserImpl(std::string_view username) : username_{username} {}

Result SqUserImpl::get_uid() const {
  if (uid_ == invalid_uid_) {
    ensure_fully_initialized();
  }
  try {
    return std::make_shared<SqIntImpl>(gsl::narrow<PrimitiveInt>(uid_));
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{
        fmt::format("cannot fit UID {} in type PrimitiveInt", uid_)};
  }
}

Result SqUserImpl::get_username() const {
  if (username_.empty()) {
    ensure_fully_initialized();
  }
  return std::make_shared<SqStringImpl>(username_);
}

Result SqUserImpl::get_group() const {
  if (gid_ == invalid_gid_) {
    ensure_fully_initialized();
  }
  return std::make_shared<SqGroupImpl>(gid_);
}

Result SqUserImpl::get_name() const {
  if (gecos_.empty()) {
    ensure_fully_initialized();
  }
  return std::make_shared<SqStringImpl>(gecos_.substr(0, gecos_.find(',')));
}

Result SqUserImpl::get_gecos() const {
  if (gecos_.empty()) {
    ensure_fully_initialized();
  }
  return FieldRange<ranges::category::forward>(
      gecos_ | ranges::views::split(',') |
      ranges::views::transform([](auto &&chars) {
        return std::make_shared<SqStringImpl>(chars |
                                              ranges::to<std::string>());
      }));
}

Result SqUserImpl::get_home() const {
  if (home_.empty()) {
    ensure_fully_initialized();
  }
  return std::make_shared<SqPathImpl>(home_);
}

Result SqUserImpl::get_shell() const {
  if (shell_.empty()) {
    ensure_fully_initialized();
  }
  return std::make_shared<SqPathImpl>(shell_);
}

Primitive SqUserImpl::to_primitive() const {
  if (uid_ == invalid_uid_) {
    ensure_fully_initialized();
  }
  try {
    return gsl::narrow<PrimitiveInt>(uid_);
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{
        fmt::format("cannot fit UID {} in type PrimitiveInt", uid_)};
  }
}

void SqUserImpl::ensure_fully_initialized() const {
  Expects(uid_ != invalid_uid_ || !username_.empty());

  if (fully_initialized_) {
    return;
  }
  const auto have_uid = uid_ != invalid_uid_;

  errno = 0;
  const auto *const pwd =
      have_uid ? getpwuid(uid_) : getpwnam(username_.c_str());

  if (pwd == nullptr) {
    const auto operation =
        have_uid ? fmt::format("getpwuid for UID {}", uid_)
                 : fmt::format("getpwnam for username {}", username_);
    if (errno == 0) {
      throw SystemError{fmt::format("{} failed: entry not found", operation)};
    }
    throw SystemError{operation, util::make_error_code(errno)};
  }

  if (have_uid) {
    ASSERT(uid_ == pwd->pw_uid);
    username_ = pwd->pw_name;
  } else {
    ASSERT(username_ == pwd->pw_name);
    uid_ = pwd->pw_uid;
  }
  gid_ = pwd->pw_gid;
  gecos_ = pwd->pw_gecos;
  home_ = pwd->pw_dir;
  shell_ = pwd->pw_shell;
  fully_initialized_ = true;
}

} // namespace sq::system::linux
