#include "field_types/SqPath.gen.h"

#include "field_types/SqBool.gen.h"
#include "field_types/SqDataSize.gen.h"
#include "field_types/SqString.gen.h"

#include <gsl/narrow>

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
    auto ret = std::vector<FieldPtr>{};
    for (const auto& dirent : std::filesystem::directory_iterator(impl_))
    {
        ret.emplace_back(create(dirent.path()));
    }
    return ret;
}

Result SqPath::get_parts() const
{
    auto ret = std::vector<FieldPtr>{};
    for (const auto& part : impl_)
    {
        ret.emplace_back(create(part.string()));
    }
    return ret;
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
