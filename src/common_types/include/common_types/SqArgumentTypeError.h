#ifndef SQ_INCLUDE_GUARD_common_types_SqArgumentTypeError_h_
#define SQ_INCLUDE_GUARD_common_types_SqArgumentTypeError_h_

#include "common_types/Primitive.h"
#include "common_types/SqException.h"

#include <string_view>

namespace sq {

class SqArgumentTypeError
    : public SqException
{
public:
    SqArgumentTypeError(const Primitive& received, std::string_view type_expected);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_SqArgumentTypeError_h_
