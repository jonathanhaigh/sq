/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/standard/SqSchemaImpl.h"

#include "system/standard/SqPrimitiveTypeSchemaImpl.h"
#include "system/standard/SqTypeSchemaImpl.h"

#include <memory>
#include <range/v3/view/transform.hpp>

namespace sq::system::standard {

Result SqSchemaImpl::get_root_type()
{
    return std::make_unique<SqTypeSchemaImpl>(*(schema().root_type));
}

Result SqSchemaImpl::get_types()
{
    return FieldRange<
        ranges::category::random_access | ranges::category::sized
    >{
        schema().types | ranges::views::transform(
            [](const TypeSchema& ts) {
                return std::make_unique<SqTypeSchemaImpl>(ts);
            }
        )
    };
}

Result SqSchemaImpl::get_primitive_types()
{
    return FieldRange<
        ranges::category::random_access | ranges::category::sized
    >{
        schema().primitive_types | ranges::views::transform(
            [](const PrimitiveTypeSchema& ps) {
                return std::make_unique<SqPrimitiveTypeSchemaImpl>(ps);
            }
        )
    };
}

Primitive SqSchemaImpl::to_primitive() const
{
    return PrimitiveString{"schema"};
}

} // namespace sq::system::standard
