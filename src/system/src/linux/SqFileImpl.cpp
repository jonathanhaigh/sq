/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqFileImpl.h"

#include "core/errors.h"
#include "system/linux/SqDataSizeImpl.h"
#include "system/linux/SqFileModeImpl.h"
#include "system/linux/SqGroupImpl.h"
#include "system/linux/SqIntImpl.h"
#include "system/linux/SqStringImpl.h"
#include "system/linux/SqTimePointImpl.h"
#include "system/linux/SqUserImpl.h"

#include <fmt/format.h>
#include <gsl/gsl>

namespace sq::system::linux {

namespace {

const char *get_file_type(const struct stat &s) {

  if (S_ISREG(s.st_mode)) {
    return "regular";
  }
  if (S_ISDIR(s.st_mode)) {
    return "directory";
  }
  if (S_ISLNK(s.st_mode)) {
    return "symlink";
  }
  if (S_ISBLK(s.st_mode)) {
    return "block";
  }
  if (S_ISCHR(s.st_mode)) {
    return "character";
  }
  if (S_ISFIFO(s.st_mode)) {
    return "fifo";
  }
  if (S_ISSOCK(s.st_mode)) {
    return "socket";
  }
  if (S_TYPEISMQ(&s)) {
    return "message queue";
  }
  if (S_TYPEISSEM(&s)) {
    return "semaphore";
  }
  if (S_TYPEISSHM(&s)) {
    return "shared memory";
  }
  return "unknown";
}

} // namespace

SqFileImpl::SqFileImpl(struct stat s, std::string_view path)
    : stat_{s}, path_{path} {}

Result SqFileImpl::get_inode() const {
  return std::make_shared<SqIntImpl>(
      to_primitive_int(stat_.st_ino, "inode number of file {}", path_));
}

Result SqFileImpl::get_size() const {
  if (S_ISREG(stat_.st_mode) || S_ISLNK(stat_.st_mode) || S_TYPEISSHM(&stat_)) {
    return std::make_shared<SqDataSizeImpl>(
        to_primitive_int(stat_.st_size, "size of file {}", stat_.st_size));
  }
  return primitive_null;
}

Result SqFileImpl::get_type() const {
  return std::make_shared<SqStringImpl>(get_file_type(stat_));
}

Result SqFileImpl::get_hard_link_count() const {
  return std::make_shared<SqIntImpl>(to_primitive_int(
      stat_.st_nlink, "hard link count of file {}", stat_.st_nlink));
}

Result SqFileImpl::get_mode() const {
  return std::make_shared<SqFileModeImpl>(stat_.st_mode & ~mode_t{S_IFMT});
}

Result SqFileImpl::get_atime() const {
  return SqTimePointImpl::from_unix_timespec(stat_.st_atim);
}

Result SqFileImpl::get_mtime() const {
  return SqTimePointImpl::from_unix_timespec(stat_.st_mtim);
}

Result SqFileImpl::get_ctime() const {
  return SqTimePointImpl::from_unix_timespec(stat_.st_ctim);
}

Result SqFileImpl::get_block_count() const {
  return std::make_shared<SqIntImpl>(
      to_primitive_int(stat_.st_blocks, "block count of file {}", path_));
}

Result SqFileImpl::get_user() const {
  return std::make_shared<SqUserImpl>(stat_.st_uid);
}

Result SqFileImpl::get_group() const {
  return std::make_shared<SqGroupImpl>(stat_.st_gid);
}

Primitive SqFileImpl::to_primitive() const {
  return to_primitive_int(stat_.st_ino, "inode number of file {}",
                          stat_.st_ino);
}

} // namespace sq::system::linux
