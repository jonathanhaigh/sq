#ifndef SQ_INCLUDE_GUARD_field_types_SqIntImpl_h_
#define SQ_INCLUDE_GUARD_field_types_SqIntImpl_h_

#include "Primitive.h"

namespace sq::field_types {

class SqIntImpl
{
public:
    explicit SqIntImpl(PrimitiveInt value);

private:
    PrimitiveInt value_;
    friend class SqInt;
};

} // namespace sq::field_types

#endif // SQ_INCLUDE_GUARD_field_types_SqIntImpl_h_
