/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqPrimitiveTypeSchemaImpl.h"
#include "system/standard/SqStringImpl.h"

#include <memory>

namespace sq::system::standard {

SqPrimitiveTypeSchemaImpl::SqPrimitiveTypeSchemaImpl(const PrimitiveTypeSchema& primitive_type_schema)
    : primitive_type_schema_{std::addressof(primitive_type_schema)}
{ }

Result SqPrimitiveTypeSchemaImpl::get_name() const
{
    return std::make_unique<SqStringImpl>(primitive_type_schema_->name());
}

Primitive SqPrimitiveTypeSchemaImpl::to_primitive() const
{
    return PrimitiveString{primitive_type_schema_->name()};
}

} // namespace sq::system::standard
