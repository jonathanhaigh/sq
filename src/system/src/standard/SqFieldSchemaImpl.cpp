/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqFieldSchemaImpl.h"

#include "system/standard/SqBoolImpl.h"
#include "system/standard/SqParamSchemaImpl.h"
#include "system/standard/SqStringImpl.h"
#include "system/standard/SqTypeSchemaImpl.h"
#include "util/typeutil.h" // for ranges::enable_view<gsl::span<T, N>>

#include <memory>
#include <range/v3/view/transform.hpp>

namespace sq::system::standard {

SqFieldSchemaImpl::SqFieldSchemaImpl(const FieldSchema &field_schema)
    : field_schema_{std::addressof(field_schema)} {}

Result SqFieldSchemaImpl::get_name() const {
  return std::make_shared<SqStringImpl>(field_schema_->name());
}

Result SqFieldSchemaImpl::get_doc() const {
  return std::make_shared<SqStringImpl>(field_schema_->doc());
}

Result SqFieldSchemaImpl::get_params() const {
  return FieldRange<ranges::category::random_access | ranges::category::sized>{
      field_schema_->params() |
      ranges::views::transform([](const ParamSchema &ps) {
        return std::make_shared<SqParamSchemaImpl>(ps);
      })};
}

Result SqFieldSchemaImpl::get_return_type() const {
  return std::make_shared<SqTypeSchemaImpl>(field_schema_->return_type());
}

Result SqFieldSchemaImpl::get_return_list() const {
  return std::make_shared<SqBoolImpl>(field_schema_->return_list());
}

Primitive SqFieldSchemaImpl::to_primitive() const {
  return PrimitiveString{field_schema_->name()};
}

} // namespace sq::system::standard
