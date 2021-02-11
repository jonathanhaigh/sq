#include "system/standard/SqRootImpl.h"

#include "system/standard/SqBoolImpl.h"
#include "system/standard/SqIntImpl.h"
#include "system/standard/SqPathImpl.h"

#include <memory>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <string>

namespace sq::system::standard {

Result SqRootImpl::get_path(const PrimitiveString* const path)
{
    if (path != nullptr)
    {
        return std::make_unique<SqPathImpl>(*path);
    }
    return std::make_unique<SqPathImpl>(std::filesystem::current_path());
}

Result SqRootImpl::get_int(const PrimitiveInt* const value)
{
    if (value != nullptr)
    {
        return std::make_unique<SqIntImpl>(*value);
    }
    return std::make_unique<SqIntImpl>(0);
}

Result SqRootImpl::get_ints(
    const PrimitiveInt* const start,
    const PrimitiveInt* const stop
)
{
    auto start_v = (start != nullptr)? *start : 0;
    auto int_to_sq_int = [](const auto i){ return std::make_unique<SqIntImpl>(i); };
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

Result SqRootImpl::get_bool(const PrimitiveBool* const value)
{
    if (value != nullptr)
    {
        return std::make_unique<SqBoolImpl>(*value);
    }
    return std::make_unique<SqBoolImpl>(false);
}

Primitive SqRootImpl::to_primitive() const
{
    return PrimitiveString("ROOT");
}

} // namespace sq::system::standard

