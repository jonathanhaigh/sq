/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqTypeSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqTypeSchemaImpl_h_

#include "core/typeutil.h"
#include "system/SqTypeSchema.gen.h"

#include "system/schema.h"

#include <gsl/gsl>

namespace sq::system::linux {

class SqTypeSchemaImpl : public SqTypeSchema<SqTypeSchemaImpl> {
public:
  explicit SqTypeSchemaImpl(const TypeSchema &type_schema);

  SQ_ND Result get_name() const;
  SQ_ND Result get_doc() const;
  SQ_ND Result get_fields() const;

  SQ_ND Primitive to_primitive() const override;

private:
  gsl::not_null<const TypeSchema *> type_schema_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqTypeSchemaImpl_h_
