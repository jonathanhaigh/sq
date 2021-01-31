#include "field_types/SqRoot.gen.h"

#include "field_types/SqBool.gen.h"
#include "field_types/SqInt.gen.h"
#include "field_types/SqPath.gen.h"

#include <string>

namespace sq::field_types {

Result SqRoot::get_path(const PrimitiveString* const path) const
{
    if (path != nullptr)
    {
        return field_types::SqPath::create(*path);
    }
    return field_types::SqPath::create(std::filesystem::current_path());
}

Result SqRoot::get_int(const PrimitiveInt* const value) const
{
    if (value != nullptr)
    {
        return field_types::SqInt::create(*value);
    }
    return field_types::SqInt::create(0);
}

Result SqRoot::get_bool(const PrimitiveBool* const value) const
{
    if (value != nullptr)
    {
        return field_types::SqBool::create(*value);
    }
    return field_types::SqBool::create(false);
}

Primitive SqRoot::to_primitive() const
{
    return std::string("ROOT");
}

} // namespace sq::field_types

