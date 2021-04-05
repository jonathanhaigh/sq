/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqPathImpl.h"

#include "core/errors.h"
#include "system/linux/SqBoolImpl.h"
#include "system/linux/SqDataSizeImpl.h"
#include "system/linux/SqFileImpl.h"
#include "system/linux/SqFileModeImpl.h"
#include "system/linux/SqIntImpl.h"
#include "system/linux/SqStringImpl.h"

#include <cerrno>
#include <range/v3/iterator_range.hpp>
#include <range/v3/view/transform.hpp>
#include <sys/stat.h>

namespace sq::system::linux {

namespace fs = std::filesystem;

namespace {

struct stat get_stat(const fs::path &path, bool follow_symlinks,
                     bool throw_if_not_exists = true) {
  struct stat s = {};
  errno = 0;
  const int ret =
      follow_symlinks ? stat(path.c_str(), &s) : lstat(path.c_str(), &s);
  if (ret == -1) {
    if (!throw_if_not_exists && (errno == ENOTDIR || errno == ENOENT)) {
      s.st_ino = 0;
      return s;
    }
    const auto *const operation = follow_symlinks ? "stat()" : "lstat()";
    throw FilesystemError{operation, path, make_error_code(errno)};
  }
  return s;
}

template <typename DirIt>
Result get_child_range(const fs::path &path, fs::directory_options opts) {
  return FieldRange<ranges::category::input>{
      ranges::iterator_range{DirIt{path, opts}, DirIt{}} |
      ranges::views::transform([](const auto &dirent) {
        return std::make_shared<SqPathImpl>(dirent.path());
      })};
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

Result SqPathImpl::get_children(PrimitiveBool recurse,
                                PrimitiveBool follow_symlinks,
                                PrimitiveBool skip_permission_denied) const {

  auto opts = fs::directory_options::none;
  if (follow_symlinks) {
    opts |= fs::directory_options::follow_directory_symlink;
  }
  if (skip_permission_denied) {
    opts |= fs::directory_options::skip_permission_denied;
  }

  return recurse
             ? get_child_range<fs::recursive_directory_iterator>(value_, opts)
             : get_child_range<fs::directory_iterator>(value_, opts);
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

Result SqPathImpl::get_exists(PrimitiveBool follow_symlinks) const {
  const auto s = get_stat(value_, follow_symlinks, false);
  return std::make_shared<SqBoolImpl>(s.st_ino != 0);
}

Result SqPathImpl::get_file(PrimitiveBool follow_symlinks) const {
  return std::make_shared<SqFileImpl>(get_stat(value_, follow_symlinks),
                                      value_.c_str());
}

Primitive SqPathImpl::to_primitive() const { return value_.string(); }

} // namespace sq::system::linux
