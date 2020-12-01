#ifndef SQ_INCLUDE_GUARD_field_types_SqBoolImpl_h_
#define SQ_INCLUDE_GUARD_field_types_SqBoolImpl_h_

#include "Primitive.h"

namespace sq::field_types {

class SqBoolImpl
{
public:
    explicit SqBoolImpl(PrimitiveBool value);

private:
    PrimitiveBool value_;
    friend class SqBool;
};

} // namespace sq::field_types

#endif // SQ_INCLUDE_GUARD_field_types_SqBoolImpl_h_
