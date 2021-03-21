/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqFileImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqFileImpl_h_

#include "core/typeutil.h"
#include "system/SqFile.gen.h"

#include <string>
#include <string_view>
#include <sys/stat.h>

namespace sq::system::linux {

class SqFileImpl : public SqFile<SqFileImpl> {
public:
  /**
   * This class represents a file, not a path.
   *
   * The path parameter is used only for error messages; for operations on
   * paths, use SqPath.
   */
  explicit SqFileImpl(struct stat s, std::string_view path = "<unknown>");

  SQ_ND Result get_inode() const;
  SQ_ND Result get_size() const;
  SQ_ND Result get_type() const;
  SQ_ND Result get_hard_link_count() const;
  SQ_ND Result get_mode() const;
  SQ_ND Result get_atime() const;
  SQ_ND Result get_mtime() const;
  SQ_ND Result get_ctime() const;
  SQ_ND Result get_block_count() const;
  SQ_ND Result get_user() const;
  SQ_ND Result get_group() const;
  SQ_ND Primitive to_primitive() const override;

private:
  struct stat stat_;
  std::string path_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqFileImpl_h_
