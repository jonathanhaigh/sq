/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqPathImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqPathImpl_h_

#include "system/SqPath.gen.h"
#include "util/typeutil.h"

#include <filesystem>
#include <sys/stat.h>

namespace sq::system::linux {

class SqPathImpl : public SqPath<SqPathImpl> {
public:
  explicit SqPathImpl(const std::filesystem::path &value);
  explicit SqPathImpl(std::filesystem::path &&value);

  SQ_ND Result get_string() const;
  SQ_ND Result get_parent() const;
  SQ_ND Result get_filename() const;
  SQ_ND Result get_extension() const;
  SQ_ND Result get_stem() const;
  SQ_ND Result get_children() const;
  SQ_ND Result get_parts() const;
  SQ_ND Result get_absolute() const;
  SQ_ND Result get_canonical() const;
  SQ_ND Result get_is_absolute() const;
  SQ_ND Result get_size(PrimitiveBool follow_symlinks) const;
  SQ_ND Result get_exists(PrimitiveBool follow_symlinks) const;
  SQ_ND Result get_type(PrimitiveBool follow_symlinks) const;
  SQ_ND Result get_hard_link_count(PrimitiveBool follow_symlinks) const;
  SQ_ND Result get_mode(PrimitiveBool follow_symlinks) const;
  SQ_ND Primitive to_primitive() const override;

private:
  const struct stat &get_stat(bool follow_symlinks) const;

  std::filesystem::path value_;
  mutable struct stat stat_ = {};
  mutable struct stat lstat_ = {};
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqPathImpl_h_
