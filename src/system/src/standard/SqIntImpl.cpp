#include "system/standard/SqIntImpl.h"

namespace sq::system::standard {

SqIntImpl::SqIntImpl(PrimitiveInt value)
    : value_{value}
{
}

Primitive SqIntImpl::to_primitive() const
{
    return value_;
}

} // namespace sq::system::standard