/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqParamSchemaImpl.h"

#include "system/standard/SqIntImpl.h"
#include "system/standard/SqPrimitiveTypeSchemaImpl.h"
#include "system/standard/SqStringImpl.h"

#include <memory>

namespace sq::system::standard {

SqParamSchemaImpl::SqParamSchemaImpl(const ParamSchema& param_schema)
    : param_schema_{std::addressof(param_schema)}
{ }

Result SqParamSchemaImpl::get_index() const
{
    return std::make_unique<SqIntImpl>(param_schema_->index);
}

Result SqParamSchemaImpl::get_name() const
{
    return std::make_unique<SqStringImpl>(param_schema_->name);
}

Result SqParamSchemaImpl::get_type() const
{
    return std::make_unique<SqPrimitiveTypeSchemaImpl>(*(param_schema_->type));
}

Primitive SqParamSchemaImpl::to_primitive() const
{
    return PrimitiveString{param_schema_->name};
}

} // namespace sq::system::standard
