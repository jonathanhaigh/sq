#include "field_types/SqFloat.gen.h"

namespace sq::field_types {

Primitive SqFloat::to_primitive() const
{
    return impl_;
}

} // namespace sq::field_types
