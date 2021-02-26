/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_

#include "system/SqRoot.gen.h"

namespace sq::system::standard {

class SqRootImpl
    : public SqRoot<SqRootImpl>
{
public:
    [[nodiscard]] static Result get_schema();
    [[nodiscard]] static Result get_path(const PrimitiveString* path);
    [[nodiscard]] static Result get_int(const PrimitiveInt* value);
    [[nodiscard]] static Result get_ints(const PrimitiveInt* start, const PrimitiveInt* stop);
    [[nodiscard]] static Result get_bool(const PrimitiveBool* value);
    [[nodiscard]] Primitive to_primitive() const override;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_
