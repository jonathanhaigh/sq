/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqTypeSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqTypeSchemaImpl_h_

#include "system/SqTypeSchema.gen.h"
#include "util/typeutil.h"

#include "system/schema.h"

#include <gsl/gsl>

namespace sq::system::standard {

class SqTypeSchemaImpl
    : public SqTypeSchema<SqTypeSchemaImpl>
{
public:
    explicit SqTypeSchemaImpl(const TypeSchema& type_schema);

    SQ_ND Result get_name() const;
    SQ_ND Result get_doc() const;
    SQ_ND Result get_fields() const;

    SQ_ND Primitive to_primitive() const override;

private:
    gsl::not_null<const TypeSchema*> type_schema_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqTypeSchemaImpl_h_
