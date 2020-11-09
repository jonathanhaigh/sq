#include "field_types/SqRoot.gen.h"
#include "field_types/SqRootImpl.h"

#include "field_types/SqInt.gen.h"
#include "field_types/SqIntImpl.h"

#include "field_types/SqPath.gen.h"
#include "field_types/SqPathImpl.h"

#include <string>

namespace sq {
namespace field_types {

Result SqRoot::get_path(const PrimitiveString* path) const
{
    if (path)
    {
        return field_types::SqPath::create(*path);
    }
    return field_types::SqPath::create(std::filesystem::current_path());
}

Result SqRoot::get_int(const PrimitiveInt* value) const
{
    if (value)
    {
        return field_types::SqInt::create(*value);
    }
    return field_types::SqInt::create(0);
}

Primitive SqRoot::to_primitive() const
{
    return std::string("ROOT");
}

} // namespace sq
} // namespace field_types

