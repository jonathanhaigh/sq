#ifndef SQ_INCLUDE_GUARD_system_standard_SqPathImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqPathImpl_h_

#include "system/SqPath.gen.h"

#include <filesystem>

namespace sq::system::standard {

class SqPathImpl
    : public SqPath<SqPathImpl>
{
public:
    SqPathImpl(const std::filesystem::path& value);
    SqPathImpl(std::filesystem::path&& value);

private:
    friend class SqPath<SqPathImpl>;

    Result get_string() const;
    Result get_parent() const;
    Result get_filename() const;
    Result get_extension() const;
    Result get_stem() const;
    Result get_children() const;
    Result get_parts() const;
    Result get_absolute() const;
    Result get_canonical() const;
    Result get_is_absolute() const;
    Result get_size() const;
    Primitive to_primitive() const;

    std::filesystem::path value_;
};


} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqPathImpl_h_
