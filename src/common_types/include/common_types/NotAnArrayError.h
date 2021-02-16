#ifndef SQ_INCLUDE_GUARD_common_types_NotAnArrayError_h_
#define SQ_INCLUDE_GUARD_common_types_NotAnArrayError_h_

#include "common_types/Exception.h"

namespace sq {

class NotAnArrayError
    : public Exception
{
public:
    using Exception::Exception;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_NotAnArrayError_h_