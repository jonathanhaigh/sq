#include "field_types/SqInt.gen.h"

namespace sq::field_types {

Primitive SqInt::to_primitive() const
{
    return impl_;
}

} // namespace sq::field_types
