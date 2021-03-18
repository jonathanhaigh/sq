/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqPathImpl.h"

#include "common_types/errors.h"
#include "system/linux/SqBoolImpl.h"
#include "system/linux/SqDataSizeImpl.h"
#include "system/linux/SqIntImpl.h"
#include "system/linux/SqStringImpl.h"

#include <cerrno>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gsl/gsl>
#include <memory>
#include <range/v3/iterator_range.hpp>
#include <range/v3/view/transform.hpp>

namespace sq::system::linux {

namespace fs = std::filesystem;

namespace {

struct stat do_stat(const fs::path &path, bool follow_symlinks) {
  errno = 0;
  struct stat s {};
  const int ret =
      follow_symlinks ? stat(path.c_str(), &s) : lstat(path.c_str(), &s);
  if (ret == -1) {
    const auto *const operation = follow_symlinks ? "stat()" : "lstat()";
    throw FilesystemError{operation, path, util::make_error_code(errno)};
  }
  return s;
}

const char *get_file_type(const struct stat &s) {

  if (S_ISREG(s.st_mode)) { // NOLINT(hicpp-signed-bitwise)
    return "regular";
  }
  if (S_ISDIR(s.st_mode)) { // NOLINT(hicpp-signed-bitwise)
    return "directory";
  }
  if (S_ISLNK(s.st_mode)) { // NOLINT(hicpp-signed-bitwise)
    return "symlink";
  }
  if (S_ISBLK(s.st_mode)) { // NOLINT(hicpp-signed-bitwise)
    return "block";
  }
  if (S_ISCHR(s.st_mode)) { // NOLINT(hicpp-signed-bitwise)
    return "character";
  }
  if (S_ISFIFO(s.st_mode)) { // NOLINT(hicpp-signed-bitwise)
    return "fifo";
  }
  if (S_ISSOCK(s.st_mode)) { // NOLINT(hicpp-signed-bitwise)
    return "socket";
  }
  if (S_TYPEISMQ(&s)) { // NOLINT(hicpp-signed-bitwise)
    return "message queue";
  }
  if (S_TYPEISSEM(&s)) { // NOLINT(hicpp-signed-bitwise)
    return "semaphore";
  }
  if (S_TYPEISSHM(&s)) { // NOLINT(hicpp-signed-bitwise)
    return "shared memory";
  }
  return "unknown";
}

} // namespace

SqPathImpl::SqPathImpl(const fs::path &value) : value_{value} {}

SqPathImpl::SqPathImpl(fs::path &&value) : value_{std::move(value)} {}

Result SqPathImpl::get_string() const {
  return std::make_shared<SqStringImpl>(value_.string());
}

Result SqPathImpl::get_parent() const {
  return std::make_shared<SqPathImpl>(value_.parent_path());
}

Result SqPathImpl::get_filename() const {
  return std::make_shared<SqStringImpl>(value_.filename().string());
}

Result SqPathImpl::get_extension() const {
  return std::make_shared<SqStringImpl>(value_.extension().string());
}

Result SqPathImpl::get_stem() const {
  return std::make_shared<SqStringImpl>(value_.stem().string());
}

Result SqPathImpl::get_children() const {
  return FieldRange<ranges::category::input>{
      ranges::iterator_range(fs::directory_iterator{value_},
                             fs::directory_iterator{}) |
      ranges::views::transform([](const auto &dirent) {
        return std::make_shared<SqPathImpl>(dirent.path());
      })};
}

Result SqPathImpl::get_parts() const {
  return FieldRange<ranges::category::bidirectional>{
      value_ | ranges::views::transform([](const auto &part) {
        return std::make_shared<SqStringImpl>(part.string());
      })};
}

Result SqPathImpl::get_absolute() const {
  return std::make_shared<SqPathImpl>(fs::absolute(value_));
}

Result SqPathImpl::get_canonical() const {
  return std::make_shared<SqPathImpl>(fs::canonical(value_));
}

Result SqPathImpl::get_is_absolute() const {
  return std::make_shared<SqBoolImpl>(value_.is_absolute());
}

Result SqPathImpl::get_size(PrimitiveBool follow_symlinks) const {
  const auto &s = get_stat(follow_symlinks);

  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if (S_ISREG(s.st_mode) || S_ISLNK(s.st_mode) || S_TYPEISSHM(&s)) {
    try {
      return std::make_shared<SqDataSizeImpl>(
          gsl::narrow<PrimitiveInt>(s.st_size));
    } catch (gsl::narrowing_error &e) {
      throw OutOfRangeError{
          fmt::format("Size of file {} ({}B) does not fit in type PrimitiveInt",
                      value_, s.st_size)};
    }
  }
  return primitive_null;
}

Result SqPathImpl::get_exists(PrimitiveBool follow_symlinks) const {
  try {
    (void)get_stat(follow_symlinks);
    return std::make_shared<SqBoolImpl>(true);
  } catch (FilesystemError &e) {
    if (e.code().value() == ENOTDIR || e.code().value() == ENOENT) {
      return std::make_shared<SqBoolImpl>(false);
    }
    throw;
  }
}

Result SqPathImpl::get_type(PrimitiveBool follow_symlinks) const {
  return std::make_shared<SqStringImpl>(
      get_file_type(get_stat(follow_symlinks)));
}

Result SqPathImpl::get_hard_link_count(PrimitiveBool follow_symlinks) const {
  const auto count = get_stat(follow_symlinks).st_nlink;
  try {
    return std::make_shared<SqIntImpl>(gsl::narrow<PrimitiveInt>(count));
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError{fmt::format(
        "Hard link count of file {} ({}) does not fit in type PrimitiveInt",
        value_, count)};
  }
}

Primitive SqPathImpl::to_primitive() const { return value_.string(); }

const struct stat &SqPathImpl::get_stat(bool follow_symlinks) const {
  auto &s = follow_symlinks ? stat_ : lstat_;
  if (s) {
    return s.value();
  }
  s = do_stat(value_, follow_symlinks);
  return s.value();
}

} // namespace sq::system::linux
