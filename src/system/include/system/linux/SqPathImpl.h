/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqPathImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqPathImpl_h_

#include "system/SqPath.gen.h"
#include "util/typeutil.h"

#include <filesystem>

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
  SQ_ND Result get_size() const;
  SQ_ND Result get_exists() const;
  SQ_ND Primitive to_primitive() const override;

private:
  std::filesystem::path value_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqPathImpl_h_
