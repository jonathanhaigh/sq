/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_

#include "system/SqRoot.gen.h"
#include "util/typeutil.h"

namespace sq::system::standard {

class SqRootImpl
    : public SqRoot<SqRootImpl>
{
public:
    SQ_ND static Result get_schema();
    SQ_ND static Result get_path(const PrimitiveString* path);
    SQ_ND static Result get_int(PrimitiveInt value);
    SQ_ND static Result get_ints(PrimitiveInt start, const PrimitiveInt* stop);
    SQ_ND static Result get_bool(PrimitiveBool value);
    SQ_ND static Result get_float(PrimitiveFloat value);
    SQ_ND Primitive to_primitive() const override;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_
