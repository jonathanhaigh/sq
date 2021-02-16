#ifndef SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_
#define SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_

#include "common_types/Primitive.h"

namespace sq::test {

Primitive to_primitive(PrimitiveString&& v);
Primitive to_primitive(const PrimitiveString& v);
Primitive to_primitive(const char* v);
Primitive to_primitive(PrimitiveInt v);
Primitive to_primitive(int v);
Primitive to_primitive(PrimitiveFloat v);
Primitive to_primitive(PrimitiveBool v);

template <typename T>
static constexpr bool is_primitive_like_v = std::disjunction_v<
    std::is_convertible<T, PrimitiveString>,
    std::is_convertible<T, PrimitiveInt>,
    std::is_convertible<T, PrimitiveFloat>,
    std::is_convertible<T, PrimitiveBool>
>;

template <typename T>
using enable_if_primitive_like_t = std::enable_if_t<is_primitive_like_v<T>>;

} // namespace sq::test

#endif // SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_
