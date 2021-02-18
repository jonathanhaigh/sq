#include "system/standard/SqBoolImpl.h"

namespace sq::system::standard {

SqBoolImpl::SqBoolImpl(PrimitiveBool value)
    : value_{value}
{
}

Primitive SqBoolImpl::to_primitive() const
{
    return value_;
}

} // namespace sq::system::standard
