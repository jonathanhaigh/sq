#include "field_types/SqBool.gen.h"

namespace sq::field_types {

Primitive SqBool::to_primitive() const
{
    return impl_;
}

} // namespace sq::field_types
