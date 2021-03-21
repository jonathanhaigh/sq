/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqPrimitiveTypeSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqPrimitiveTypeSchemaImpl_h_

#include "core/typeutil.h"
#include "system/SqPrimitiveTypeSchema.gen.h"
#include "system/schema.h"

#include <gsl/gsl>

namespace sq::system::linux {

class SqPrimitiveTypeSchemaImpl
    : public SqPrimitiveTypeSchema<SqPrimitiveTypeSchemaImpl> {
public:
  explicit SqPrimitiveTypeSchemaImpl(
      const PrimitiveTypeSchema &primitive_type_schema);

  SQ_ND Result get_name() const;
  SQ_ND Result get_doc() const;

  SQ_ND Primitive to_primitive() const override;

private:
  gsl::not_null<const PrimitiveTypeSchema *> primitive_type_schema_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqPrimitiveTypeSchemaImpl_h_
