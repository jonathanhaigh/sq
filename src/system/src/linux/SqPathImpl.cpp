/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqPathImpl.h"

#include "common_types/errors.h"
#include "system/linux/SqBoolImpl.h"
#include "system/linux/SqDataSizeImpl.h"
#include "system/linux/SqStringImpl.h"

#include <filesystem>
#include <fmt/format.h>
#include <gsl/gsl>
#include <memory>
#include <range/v3/iterator_range.hpp>
#include <range/v3/view/transform.hpp>

namespace sq::system::linux {

namespace fs = std::filesystem;

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

Result SqPathImpl::get_size() const {

  // std::filesystem::file_size(path) has implementation defined behaviour if
  // path is not a regular file or a symlink. We want to be predictable, so
  // check the type of the file first and return null if it's not a regular
  // file or a symlink.

  auto type = fs::file_type::none;
  try {
    type = fs::status(value_).type();
  } catch (fs::filesystem_error &e) {
    throw FilesystemError{fmt::format("Failed to get status of file \"{}\": {}",
                                      value_.string(), e.what())};
  }

  if (type == fs::file_type::none) {
    throw FilesystemError{
        fmt::format("Failed to get status of file \"{}\"", value_.string())};
  }

  if (type == fs::file_type::not_found) {
    throw FileNotFoundError{value_};
  }

  if (type != fs::file_type::regular && type != fs::file_type::symlink) {
    return primitive_null;
  }

  std::uintmax_t size = 0;
  try {
    size = fs::file_size(value_);
    return std::make_shared<SqDataSizeImpl>(gsl::narrow<PrimitiveInt>(size));
  } catch (fs::filesystem_error &e) {
    throw FilesystemError{fmt::format("Failed to get size of file \"{}\": {}",
                                      value_.string(), e.what())};
  } catch (gsl::narrowing_error &e) {
    throw OutOfRangeError(fmt::format(
        "Size of file \"{}\" ({}B) does not fit in type PrimitiveInt",
        value_.string(), size));
  }
}

Primitive SqPathImpl::to_primitive() const { return value_.string(); }

} // namespace sq::system::linux
