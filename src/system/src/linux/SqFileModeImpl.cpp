/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqFileModeImpl.h"

#include "system/linux/SqBoolImpl.h"
#include "system/linux/SqIntImpl.h"

#include <sys/stat.h>

namespace sq::system::linux {

SqFileModeImpl::SqFileModeImpl(mode_t value) : value_{value} {}

Result SqFileModeImpl::get_permissions() const {
  return std::make_shared<SqIntImpl>(
      value_ & (mode_t{S_IRWXU} | mode_t{S_IRWXG} | mode_t{S_IRWXO}));
}

Result SqFileModeImpl::get_suid() const {
  return std::make_shared<SqBoolImpl>(value_ & mode_t{S_ISUID});
}

Result SqFileModeImpl::get_sgid() const {
  return std::make_shared<SqBoolImpl>(value_ & mode_t{S_ISGID});
}

Result SqFileModeImpl::get_sticky() const {
  return std::make_shared<SqBoolImpl>(value_ & mode_t{S_ISVTX});
}

Primitive SqFileModeImpl::to_primitive() const {
  return gsl::narrow<PrimitiveInt>(value_);
}

} // namespace sq::system::linux
