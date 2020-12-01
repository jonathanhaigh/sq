#include "field_types/SqBool.gen.h"
#include "field_types/SqBoolImpl.h"

namespace sq::field_types {

Primitive SqBool::to_primitive() const
{
    return impl_->value_;
}

} // namespace sq::field_types
