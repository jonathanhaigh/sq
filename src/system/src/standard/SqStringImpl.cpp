#include "system/standard/SqStringImpl.h"

namespace sq::system::standard {

SqStringImpl::SqStringImpl(const PrimitiveString& value)
    : value_{value}
{
}

Primitive SqStringImpl::to_primitive() const
{
    return value_;
}

} // namespace sq::system::standard
