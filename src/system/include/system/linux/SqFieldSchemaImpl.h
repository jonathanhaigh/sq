/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqFieldSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqFieldSchemaImpl_h_

#include "system/SqFieldSchema.gen.h"
#include "system/schema.h"
#include "util/typeutil.h"

#include <gsl/gsl>

namespace sq::system::linux {

class SqFieldSchemaImpl : public SqFieldSchema<SqFieldSchemaImpl> {
public:
  explicit SqFieldSchemaImpl(const FieldSchema &field_schema);

  SQ_ND Result get_name() const;
  SQ_ND Result get_doc() const;
  SQ_ND Result get_params() const;
  SQ_ND Result get_return_type() const;
  SQ_ND Result get_return_list() const;
  SQ_ND Result get_null() const;

  SQ_ND Primitive to_primitive() const override;

private:
  gsl::not_null<const FieldSchema *> field_schema_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqFieldSchemaImpl_h_
