/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqParamSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqParamSchemaImpl_h_

#include "system/SqParamSchema.gen.h"
#include "system/schema.h"
#include "util/typeutil.h"

#include <gsl/gsl>

namespace sq::system::linux {

class SqParamSchemaImpl : public SqParamSchema<SqParamSchemaImpl> {
public:
  explicit SqParamSchemaImpl(const ParamSchema &param_schema);

  SQ_ND Result get_name() const;
  SQ_ND Result get_doc() const;
  SQ_ND Result get_index() const;
  SQ_ND Result get_type() const;
  SQ_ND Result get_required() const;
  SQ_ND Result get_default_value() const;
  SQ_ND Result get_default_value_doc() const;

  SQ_ND Primitive to_primitive() const override;

private:
  gsl::not_null<const ParamSchema *> param_schema_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqParamSchemaImpl_h_
