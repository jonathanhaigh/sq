#ifndef SQ_INCLUDE_GUARD_field_types_SqPathImpl_h_
#define SQ_INCLUDE_GUARD_field_types_SqPathImpl_h_

#include <filesystem>
#include <utility>

namespace sq {
namespace field_types {

class SqPathImpl
{
public:
    SqPathImpl() = default;
    SqPathImpl(const SqPathImpl&) = default;
    SqPathImpl(SqPathImpl&&) = default;
    SqPathImpl& operator=(const SqPathImpl&) = default;
    SqPathImpl& operator=(SqPathImpl&&) = default;

    template <typename... Args>
    explicit SqPathImpl(Args&&... args)
        : path_{std::forward<Args>(args)...}
    { }

private:
    std::filesystem::path path_;
    friend class SqPath;
};

} // namespace field_types
} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_SqPathImpl_h_
