#include "field_types/SqString.gen.h"

namespace sq::field_types {

Primitive SqString::to_primitive() const
{
    return impl_;
}

} // namespace sq::field_types
