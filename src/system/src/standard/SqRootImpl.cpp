/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqRootImpl.h"

#include "system/standard/SqBoolImpl.h"
#include "system/standard/SqDataSizeImpl.h"
#include "system/standard/SqFloatImpl.h"
#include "system/standard/SqIntImpl.h"
#include "system/standard/SqPathImpl.h"
#include "system/standard/SqSchemaImpl.h"
#include "system/standard/SqStringImpl.h"

#include <memory>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <string>

namespace sq::system::standard {

Result SqRootImpl::get_schema() { return std::make_shared<SqSchemaImpl>(); }

Result SqRootImpl::get_path(const std::optional<PrimitiveString> &path) {
  if (path != std::nullopt) {
    return std::make_shared<SqPathImpl>(path.value());
  }
  return std::make_shared<SqPathImpl>(std::filesystem::current_path());
}

Result SqRootImpl::get_int(PrimitiveInt value) {
  return std::make_shared<SqIntImpl>(value);
}

Result SqRootImpl::get_ints(PrimitiveInt start,
                            const std::optional<PrimitiveInt> &stop) {
  auto int_to_sq_int = [](const auto i) {
    return std::make_shared<SqIntImpl>(i);
  };
  if (stop != std::nullopt) {
    return FieldRange<ranges::category::bidirectional |
                      ranges::category::sized>{
        ranges::views::iota(start, stop.value()) |
        ranges::views::transform(int_to_sq_int)};
  }
  return FieldRange<ranges::category::bidirectional>{
      ranges::views::iota(start, ranges::unreachable) |
      ranges::views::transform(int_to_sq_int)};
}

Result SqRootImpl::get_bool(PrimitiveBool value) {
  return std::make_shared<SqBoolImpl>(value);
}

Result SqRootImpl::get_float(PrimitiveFloat value) {
  return std::make_shared<SqFloatImpl>(value);
}

Result SqRootImpl::get_string(const PrimitiveString &value) {
  return std::make_shared<SqStringImpl>(value);
}

Result SqRootImpl::get_data_size(PrimitiveInt bytes) {
  return std::make_shared<SqDataSizeImpl>(bytes);
}

Primitive SqRootImpl::to_primitive() const { return PrimitiveString("ROOT"); }

} // namespace sq::system::standard
