#ifndef SQ_INCLUDE_GUARD_system_standard_SqIntImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqIntImpl_h_

#include "schema/SqInt.gen.h"

namespace sq::system::standard {

class SqIntImpl
    : public schema::SqInt<SqIntImpl>
{
public:
    explicit SqIntImpl(PrimitiveInt value);

private:
    friend class schema::SqInt<SqIntImpl>;

    Primitive to_primitive() const;

    PrimitiveInt value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqIntImpl_h_
