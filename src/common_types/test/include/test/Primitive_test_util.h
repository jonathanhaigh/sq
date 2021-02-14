#ifndef SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_
#define SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_

#include "common_types/Primitive.h"

namespace sq::test {

template <typename T>
Primitive to_primitive(T&& v);

Primitive to_primitive(int v);
Primitive to_primitive(const char* v);

} // namespace sq::test

#include "Primitive_test_util.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_test_Primitive_test_util_h_
