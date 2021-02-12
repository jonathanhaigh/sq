#ifndef SQ_INCLUDE_GUARD_system_standard_SqFloatImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqFloatImpl_h_

#include "system/SqFloat.gen.h"

namespace sq::system::standard {

class SqFloatImpl
    : public SqFloat<SqFloatImpl>
{
public:
    explicit SqFloatImpl(PrimitiveFloat value);

private:
    friend class SqFloat<SqFloatImpl>;

    Primitive to_primitive() const;

    PrimitiveFloat value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqFloatImpl_h_
