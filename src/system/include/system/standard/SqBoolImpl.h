#ifndef SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_

#include "schema/SqBool.gen.h"

namespace sq::system::standard {

class SqBoolImpl
    : public schema::SqBool<SqBoolImpl>
{
public:
    explicit SqBoolImpl(PrimitiveBool value);

private:
    friend class schema::SqBool<SqBoolImpl>;

    Primitive to_primitive() const override;

    PrimitiveBool value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_
