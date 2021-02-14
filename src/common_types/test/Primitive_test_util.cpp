#include "test/Primitive_test_util.h"

namespace sq::test {

Primitive to_primitive(int v)
{
    return static_cast<PrimitiveInt>(v);
}

Primitive to_primitive(const char* v)
{
    return PrimitiveString(v);
}

} // namespace sq::test
