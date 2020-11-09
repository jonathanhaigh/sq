#ifndef SQ_INCLUDE_GUARD_field_types_SqStringImpl_h_
#define SQ_INCLUDE_GUARD_field_types_SqStringImpl_h_

#include "Primitive.h"

namespace sq::field_types {

class SqStringImpl
{
public:
    explicit SqStringImpl(const PrimitiveString& str);

private:
    PrimitiveString string_;
    friend class SqString;
};

} // namespace sq::field_types

#endif // SQ_INCLUDE_GUARD_field_types_SqStringImpl_h_
