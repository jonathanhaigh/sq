#include "field_types/SqRoot.gen.h"

#include "field_types/SqBool.gen.h"
#include "field_types/SqInt.gen.h"
#include "field_types/SqPath.gen.h"

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

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

Result SqRoot::get_ints(
    const PrimitiveInt* const start,
    const PrimitiveInt* const stop
) const
{
    auto start_v = (start != nullptr)? *start : 0;
    auto int_to_sq_int = [](const auto i){ return SqInt::create(i); };
    if (stop != nullptr)
    {
        return FieldRange<ranges::category::bidirectional | ranges::category::sized>{
            ranges::views::iota(start_v, *stop) |
            ranges::views::transform(int_to_sq_int)
        };
    }
    return FieldRange<ranges::category::bidirectional>{
        ranges::views::iota(start_v, ranges::unreachable) |
        ranges::views::transform(int_to_sq_int)
    };
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

