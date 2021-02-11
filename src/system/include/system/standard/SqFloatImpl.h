#ifndef SQ_INCLUDE_GUARD_system_standard_SqFloatImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqFloatImpl_h_

#include "schema/SqFloat.gen.h"

namespace sq::system::standard {

class SqFloatImpl
    : public schema::SqFloat<SqFloatImpl>
{
public:
    explicit SqFloatImpl(PrimitiveFloat value);

private:
    friend class schema::SqFloat<SqFloatImpl>;

    Primitive to_primitive() const;

    PrimitiveFloat value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqFloatImpl_h_
