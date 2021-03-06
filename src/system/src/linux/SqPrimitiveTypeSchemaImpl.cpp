/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/SqPrimitiveTypeSchemaImpl.h"
#include "system/linux/SqStringImpl.h"

#include <memory>

namespace sq::system::linux {

SqPrimitiveTypeSchemaImpl::SqPrimitiveTypeSchemaImpl(
    const PrimitiveTypeSchema &primitive_type_schema)
    : primitive_type_schema_{std::addressof(primitive_type_schema)} {}

Result SqPrimitiveTypeSchemaImpl::get_name() const {
  return std::make_shared<SqStringImpl>(primitive_type_schema_->name());
}

Result SqPrimitiveTypeSchemaImpl::get_doc() const {
  return std::make_shared<SqStringImpl>(primitive_type_schema_->doc());
}

Primitive SqPrimitiveTypeSchemaImpl::to_primitive() const {
  return PrimitiveString{primitive_type_schema_->name()};
}

} // namespace sq::system::linux
