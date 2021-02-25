/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_

#include "system/SqString.gen.h"

namespace sq::system::standard {

class SqStringImpl
    : public SqString<SqStringImpl>
{
public:
    explicit SqStringImpl(const PrimitiveString& value);

    [[nodiscard]] Primitive to_primitive() const override;

private:
    PrimitiveString value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_
