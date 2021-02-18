#ifndef SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_inl_h_
#define SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_inl_h_

#include "test/Primitive_test_util.h"

#include <utility>

namespace sq::test {

namespace detail {

template <typename T>
void add_params(FieldCallParams& fcp, T&& p)
{
    fcp.pos_params().emplace_back(to_primitive(std::forward<T>(p)));
}

inline void add_params(FieldCallParams& fcp, NamedParam&& p)
{
    fcp.named_params().emplace(std::move(p));
}

template <typename T, typename... Args>
void add_params(FieldCallParams& fct, T&& p, Args&&... args)
{
    add_params(fct, std::forward<T>(p));
    add_params(fct, std::forward<Args>(args)...);
}

} // namespace detail

template <typename... Args>
FieldCallParams params(Args&&... args)
{
    auto ret = FieldCallParams{};
    detail::add_params(ret, std::forward<Args>(args)...);
    return ret;
}

template <typename T>
NamedParam named(const char* name, T&& np)
{
    return NamedParam{name, to_primitive(std::forward<T>(np))};
}

} // namespace sq::test

#endif // SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_inl_h_
