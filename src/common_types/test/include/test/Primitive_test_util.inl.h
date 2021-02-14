#ifndef SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_inl_h_
#define SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_inl_h_

namespace sq::test {

template <typename T>
Primitive to_primitive(T&& v)
{
    return std::forward<T>(v);
}

} // namespace sq::test

#endif // SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_inl_h_
