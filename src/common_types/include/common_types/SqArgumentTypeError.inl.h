#ifndef SQ_INCLUDE_GUARD_common_types_SqArgumentTypeError_inl_h_
#define SQ_INCLUDE_GUARD_common_types_SqArgumentTypeError_inl_h_

#include <sstream>

namespace sq {

template <typename Expected>
SqArgumentTypeError SqArgumentTypeError::create(const Primitive& received)
{
    return SqArgumentTypeError{received, primitive_type_name_v<Expected>};
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_SqArgumentTypeError_inl_h_

