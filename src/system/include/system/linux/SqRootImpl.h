/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqRootImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqRootImpl_h_

#include "system/SqRoot.gen.h"
#include "util/typeutil.h"

namespace sq::system::linux {

class SqRootImpl : public SqRoot<SqRootImpl> {
public:
  SQ_ND static Result get_schema();
  SQ_ND static Result get_path(const std::optional<PrimitiveString> &path);
  SQ_ND static Result get_int(PrimitiveInt value);
  SQ_ND static Result get_ints(PrimitiveInt start,
                               const std::optional<PrimitiveInt> &stop);
  SQ_ND static Result get_bool(PrimitiveBool value);
  SQ_ND static Result get_float(PrimitiveFloat value);
  SQ_ND static Result get_string(const PrimitiveString &value);
  SQ_ND static Result get_data_size(PrimitiveInt bytes);
  SQ_ND static Result get_devices();
  SQ_ND Primitive to_primitive() const override;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqRootImpl_h_
