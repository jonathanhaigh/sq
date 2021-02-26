/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqFieldSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqFieldSchemaImpl_h_

#include "system/SqFieldSchema.gen.h"

#include "system/schema.h"

namespace sq::system::standard {

class SqFieldSchemaImpl
    : public SqFieldSchema<SqFieldSchemaImpl>
{
public:
    explicit SqFieldSchemaImpl(const FieldSchema& field_schema);

    [[nodiscard]] Result get_name() const;
    [[nodiscard]] Result get_params() const;
    [[nodiscard]] Result get_return_type() const;
    [[nodiscard]] Result get_return_list() const;

    [[nodiscard]] Primitive to_primitive() const override;

private:
    const FieldSchema* field_schema_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqFieldSchemaImpl_h_
