/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqParamSchemaImpl.h"

#include "system/linux/SqBoolImpl.h"
#include "system/linux/SqIntImpl.h"
#include "system/linux/SqPrimitiveTypeSchemaImpl.h"
#include "system/linux/SqStringImpl.h"

#include <memory>

namespace sq::system::linux {

SqParamSchemaImpl::SqParamSchemaImpl(const ParamSchema &param_schema)
    : param_schema_{std::addressof(param_schema)} {}

Result SqParamSchemaImpl::get_name() const {
  return std::make_shared<SqStringImpl>(param_schema_->name());
}

Result SqParamSchemaImpl::get_doc() const {
  return std::make_shared<SqStringImpl>(param_schema_->doc());
}

Result SqParamSchemaImpl::get_index() const {
  return std::make_shared<SqIntImpl>(param_schema_->index());
}

Result SqParamSchemaImpl::get_type() const {
  return std::make_shared<SqPrimitiveTypeSchemaImpl>(param_schema_->type());
}

Result SqParamSchemaImpl::get_required() const {
  return std::make_shared<SqBoolImpl>(param_schema_->required());
}

Primitive SqParamSchemaImpl::to_primitive() const {
  return PrimitiveString{param_schema_->name()};
}

} // namespace sq::system::linux
