/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqIntImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqIntImpl_h_

#include "system/SqInt.gen.h"

namespace sq::system::standard {

class SqIntImpl
    : public SqInt<SqIntImpl>
{
public:
    explicit SqIntImpl(PrimitiveInt value);

    [[nodiscard]] Primitive to_primitive() const override;

private:
    PrimitiveInt value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqIntImpl_h_
