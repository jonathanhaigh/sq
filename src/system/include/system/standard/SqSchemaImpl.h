/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqSchemaImpl_h_

#include "system/SqSchema.gen.h"

#include "system/schema.h"

namespace sq::system::standard {

class SqSchemaImpl
    : public SqSchema<SqSchemaImpl>
{
public:
    [[nodiscard]] static Result get_primitive_types();
    [[nodiscard]] static Result get_types();
    [[nodiscard]] static Result get_root_type();

    [[nodiscard]] Primitive to_primitive() const override;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqSchemaImpl_h_
