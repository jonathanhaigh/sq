/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqParamSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqParamSchemaImpl_h_

#include "system/SqParamSchema.gen.h"

#include "system/schema.h"

namespace sq::system::standard {

class SqParamSchemaImpl
    : public SqParamSchema<SqParamSchemaImpl>
{
public:
    explicit SqParamSchemaImpl(const ParamSchema& param_schema);

    [[nodiscard]] Result get_index() const;
    [[nodiscard]] Result get_name() const;
    [[nodiscard]] Result get_type() const;

    [[nodiscard]] Primitive to_primitive() const override;

private:
    const ParamSchema* param_schema_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqParamSchemaImpl_h_
