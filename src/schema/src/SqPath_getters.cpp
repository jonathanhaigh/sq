#include "field_types/SqPath.gen.h"

#include "field_types/SqBool.gen.h"
#include "field_types/SqDataSize.gen.h"
#include "field_types/SqString.gen.h"

#include <gsl/narrow>
#include <range/v3/iterator_range.hpp>
#include <range/v3/view/transform.hpp>

namespace sq::field_types {

Result SqPath::get_string() const
{
    return SqString::create(impl_.string());
}

Result SqPath::get_parent() const
{
    return SqPath::create(impl_.parent_path());
}

Result SqPath::get_filename() const
{
    return SqString::create(impl_.filename().string());
}

Result SqPath::get_extension() const
{
    return SqString::create(impl_.extension().string());
}

Result SqPath::get_stem() const
{
    return SqString::create(impl_.stem().string());
}

Result SqPath::get_children() const
{
    return FieldRange<ranges::category::input>{
        ranges::iterator_range(
            std::filesystem::directory_iterator{impl_},
            std::filesystem::directory_iterator{}
        ) |
        ranges::views::transform(
            [](const auto& dirent){ return create(dirent.path()); }
        )
    };
}

Result SqPath::get_parts() const
{
    return FieldRange<ranges::category::bidirectional>{
        impl_ |
        ranges::views::transform(
            [](const auto& part){ return SqString::create(part.string()); }
        )
    };
}

Result SqPath::get_absolute() const
{
    return create(std::filesystem::absolute(impl_));
}

Result SqPath::get_canonical() const
{
    return create(std::filesystem::canonical(impl_));
}

Result SqPath::get_is_absolute() const
{
    return SqBool::create(impl_.is_absolute());
}

Result SqPath::get_size() const
{
    return SqDataSize::create(gsl::narrow<std::size_t>(std::filesystem::file_size(impl_)));
}

Primitive SqPath::to_primitive() const
{
    return impl_.string();
}

} // namespace sq::field_types
