#include "field_types/SqPath.gen.h"
#include "field_types/SqPathImpl.h"

#include "field_types/SqString.gen.h"
#include "field_types/SqStringImpl.h"

namespace sq {
namespace field_types {

Result SqPath::get_string() const
{
    return SqString::create(impl_->path_.string());
}

Result SqPath::get_parent() const
{
    return SqPath::create(impl_->path_.parent_path());
}

Result SqPath::get_filename() const
{
    return SqString::create(impl_->path_.filename().string());
}

Result SqPath::get_extension() const
{
    return SqString::create(impl_->path_.extension().string());
}

Result SqPath::get_stem() const
{
    return SqString::create(impl_->path_.stem().string());
}

Result SqPath::get_children() const
{
    std::vector<FieldPtr> ret;
    for (const auto& dirent : std::filesystem::directory_iterator(impl_->path_))
    {
        ret.emplace_back(create(dirent.path()));
    }
    return ret;
}

Primitive SqPath::to_primitive() const
{
    return impl_->path_.string();
}

} // namespace sq
} // namespace field_types
