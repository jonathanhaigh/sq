/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqTypeSchemaImpl.h"

#include "system/standard/SqFieldSchemaImpl.h"
#include "system/standard/SqStringImpl.h"
#include "util/typeutil.h" // for ranges::enable_view<gsl::span<T, N>>

#include <memory>
#include <range/v3/view/transform.hpp>

namespace sq::system::standard {

SqTypeSchemaImpl::SqTypeSchemaImpl(const TypeSchema& type_schema)
    : type_schema_{std::addressof(type_schema)}
{ }

Result SqTypeSchemaImpl::get_name() const
{
    return std::make_unique<SqStringImpl>(type_schema_->name());
}

Result SqTypeSchemaImpl::get_fields() const
{
    return FieldRange<
        ranges::category::random_access | ranges::category::sized
    >{
        type_schema_->fields() | ranges::views::transform(
            [](const FieldSchema& fs) {
                return std::make_unique<SqFieldSchemaImpl>(fs);
            }
        )
    };
}

Primitive SqTypeSchemaImpl::to_primitive() const
{
    return PrimitiveString{type_schema_->name()};
}

} // namespace sq::system::standard
