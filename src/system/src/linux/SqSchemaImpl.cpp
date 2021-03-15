/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqSchemaImpl.h"

#include "system/linux/SqPrimitiveTypeSchemaImpl.h"
#include "system/linux/SqTypeSchemaImpl.h"
#include "util/typeutil.h" // for ranges::enable_view<gsl::span<T, N>>

#include <memory>
#include <range/v3/view/transform.hpp>

namespace sq::system::linux {

Result SqSchemaImpl::get_root_type() {
  return std::make_shared<SqTypeSchemaImpl>(schema().root_type());
}

Result SqSchemaImpl::get_types() {
  return FieldRange<ranges::category::random_access | ranges::category::sized>{
      schema().types() | ranges::views::transform([](const TypeSchema &ts) {
        return std::make_shared<SqTypeSchemaImpl>(ts);
      })};
}

Result SqSchemaImpl::get_primitive_types() {
  return FieldRange<ranges::category::random_access | ranges::category::sized>{
      schema().primitive_types() |
      ranges::views::transform([](const PrimitiveTypeSchema &ps) {
        return std::make_shared<SqPrimitiveTypeSchemaImpl>(ps);
      })};
}

Primitive SqSchemaImpl::to_primitive() const {
  return PrimitiveString{"schema"};
}

} // namespace sq::system::linux
