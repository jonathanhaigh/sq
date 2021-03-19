/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqFileImpl.h"

#include "common_types/errors.h"
#include "system/linux/SqDataSizeImpl.h"
#include "system/linux/SqFileModeImpl.h"
#include "system/linux/SqIntImpl.h"
#include "system/linux/SqStringImpl.h"

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
  try {
    return std::make_shared<SqIntImpl>(gsl::narrow<PrimitiveInt>(stat_.st_ino));
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{fmt::format(
        "inode number of file {} ({}) does not fit in type PrimitiveInt", path_,
        stat_.st_ino)};
  }
}

Result SqFileImpl::get_size() const {
  if (S_ISREG(stat_.st_mode) || S_ISLNK(stat_.st_mode) || S_TYPEISSHM(&stat_)) {
    try {
      return std::make_shared<SqDataSizeImpl>(
          gsl::narrow<PrimitiveInt>(stat_.st_size));
    } catch (gsl::narrowing_error &e) {
      throw OutOfRangeError{
          fmt::format("size of file {} ({}B) does not fit in type PrimitiveInt",
                      path_, stat_.st_size)};
    }
  }
  return primitive_null;
}

Result SqFileImpl::get_type() const {
  return std::make_shared<SqStringImpl>(get_file_type(stat_));
}

Result SqFileImpl::get_hard_link_count() const {
  try {
    return std::make_shared<SqIntImpl>(
        gsl::narrow<PrimitiveInt>(stat_.st_nlink));
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{fmt::format(
        "hard link count of file {} ({}) does not fit in type PrimitiveInt",
        path_, stat_.st_nlink)};
  }
}

Result SqFileImpl::get_mode() const {
  return std::make_shared<SqFileModeImpl>(stat_.st_mode & ~mode_t{S_IFMT});
}

Primitive SqFileImpl::to_primitive() const {
  try {
    return gsl::narrow<PrimitiveInt>(stat_.st_ino);
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{fmt::format(
        "inode number of file {} ({}) does not fit in type PrimitiveInt", path_,
        stat_.st_ino)};
  }
}

} // namespace sq::system::linux
