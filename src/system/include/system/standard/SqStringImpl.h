#ifndef SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_

#include "schema/SqString.gen.h"

namespace sq::system::standard {

class SqStringImpl
    : public schema::SqString<SqStringImpl>
{
public:
    explicit SqStringImpl(const PrimitiveString& value);

private:
    friend class schema::SqString<SqStringImpl>;

    Primitive to_primitive() const;

    PrimitiveString value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqStringImpl_h_
