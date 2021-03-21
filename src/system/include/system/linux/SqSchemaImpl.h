/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqSchemaImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqSchemaImpl_h_

#include "core/typeutil.h"
#include "system/SqSchema.gen.h"
#include "system/schema.h"

namespace sq::system::linux {

class SqSchemaImpl : public SqSchema<SqSchemaImpl> {
public:
  SQ_ND static Result get_primitive_types();
  SQ_ND static Result get_types();
  SQ_ND static Result get_root_type();

  SQ_ND Primitive to_primitive() const override;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqSchemaImpl_h_
