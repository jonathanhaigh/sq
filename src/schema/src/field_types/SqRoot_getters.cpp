#include "field_types/SqRoot.gen.h"
#include "field_types/SqRootImpl.h"

#include "field_types/SqPath.gen.h"
#include "field_types/SqPathImpl.h"

#include <string>

namespace sq {
namespace field_types {

Result SqRoot::get_path() const
{
    return field_types::SqPath::create(std::filesystem::current_path());
}

Primitive SqRoot::to_primitive() const
{
    return std::string("ROOT");
}

} // namespace sq
} // namespace field_types

