#include "field_types/SqString.gen.h"
#include "field_types/SqStringImpl.h"

namespace sq {
namespace field_types {

Primitive SqString::to_primitive() const
{
    return impl_->string_;
}

} // namespace sq
} // namespace field_types
