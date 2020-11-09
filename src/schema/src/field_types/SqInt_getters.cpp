#include "field_types/SqInt.gen.h"
#include "field_types/SqIntImpl.h"

namespace sq::field_types {

Primitive SqInt::to_primitive() const
{
    return impl_->value_;
}

} // namespace sq::field_types
