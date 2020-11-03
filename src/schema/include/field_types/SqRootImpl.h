#ifndef SQ_INCLUDE_GUARD_field_types_SqRootImpl_h_
#define SQ_INCLUDE_GUARD_field_types_SqRootImpl_h_

namespace sq {
namespace field_types {

class SqRootImpl
{
public:
    SqRootImpl() = default;
    SqRootImpl(const SqRootImpl&) = default;
    SqRootImpl(SqRootImpl&&) = default;
    SqRootImpl& operator=(const SqRootImpl&) = default;
    SqRootImpl& operator=(SqRootImpl&&) = default;

private:
    friend class SqRoot;
};

} // namespace field_types
} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_SqRootImpl_h_
