/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqPrimitiveTypeSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqPrimitiveTypeSchemaImpl_h_

#include "system/SqPrimitiveTypeSchema.gen.h"

#include "system/schema.h"

#include <gsl/gsl>

namespace sq::system::standard {

class SqPrimitiveTypeSchemaImpl
    : public SqPrimitiveTypeSchema<SqPrimitiveTypeSchemaImpl>
{
public:
    explicit SqPrimitiveTypeSchemaImpl(const PrimitiveTypeSchema& primitive_type_schema);

    [[nodiscard]] Result get_name() const;
    [[nodiscard]] Result get_doc() const;

    [[nodiscard]] Primitive to_primitive() const override;

private:
    gsl::not_null<const PrimitiveTypeSchema*> primitive_type_schema_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqPrimitiveTypeSchemaImpl_h_
