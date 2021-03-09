/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqPathImpl.h"

#include "system/standard/SqBoolImpl.h"
#include "system/standard/SqDataSizeImpl.h"
#include "system/standard/SqStringImpl.h"

#include <gsl/gsl>
#include <memory>
#include <range/v3/iterator_range.hpp>
#include <range/v3/view/transform.hpp>

namespace sq::system::standard {

SqPathImpl::SqPathImpl(const std::filesystem::path &value) : value_{value} {}

SqPathImpl::SqPathImpl(std::filesystem::path &&value)
    : value_{std::move(value)} {}

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
      ranges::iterator_range(std::filesystem::directory_iterator{value_},
                             std::filesystem::directory_iterator{}) |
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
  return std::make_shared<SqPathImpl>(std::filesystem::absolute(value_));
}

Result SqPathImpl::get_canonical() const {
  return std::make_shared<SqPathImpl>(std::filesystem::canonical(value_));
}

Result SqPathImpl::get_is_absolute() const {
  return std::make_shared<SqBoolImpl>(value_.is_absolute());
}

Result SqPathImpl::get_size() const {
  return std::make_shared<SqDataSizeImpl>(
      gsl::narrow<PrimitiveInt>(std::filesystem::file_size(value_)));
}

Primitive SqPathImpl::to_primitive() const { return value_.string(); }

} // namespace sq::system::standard
