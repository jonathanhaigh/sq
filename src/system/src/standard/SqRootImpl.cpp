/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqRootImpl.h"

#include "system/standard/SqBoolImpl.h"
#include "system/standard/SqFloatImpl.h"
#include "system/standard/SqIntImpl.h"
#include "system/standard/SqPathImpl.h"
#include "system/standard/SqSchemaImpl.h"

#include <memory>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <string>

namespace sq::system::standard {

Result SqRootImpl::get_schema() { return std::make_shared<SqSchemaImpl>(); }

Result SqRootImpl::get_path(const PrimitiveString *path) {
  if (path != nullptr) {
    return std::make_shared<SqPathImpl>(*path);
  }
  return std::make_shared<SqPathImpl>(std::filesystem::current_path());
}

Result SqRootImpl::get_int(PrimitiveInt value) {
  return std::make_shared<SqIntImpl>(value);
}

Result SqRootImpl::get_ints(PrimitiveInt start, const PrimitiveInt *stop) {
  auto int_to_sq_int = [](const auto i) {
    return std::make_shared<SqIntImpl>(i);
  };
  if (stop != nullptr) {
    return FieldRange<ranges::category::bidirectional |
                      ranges::category::sized>{
        ranges::views::iota(start, *stop) |
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

Primitive SqRootImpl::to_primitive() const { return PrimitiveString("ROOT"); }

} // namespace sq::system::standard
