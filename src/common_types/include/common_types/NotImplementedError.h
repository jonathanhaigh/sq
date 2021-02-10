#ifndef SQ_INCLUDE_GUARD_common_types_NotImplementedError_h_
#define SQ_INCLUDE_GUARD_common_types_NotImplementedError_h_

#include "common_types/SqException.h"

namespace sq {

class NotImplementedError
    : public SqException
{
public:
    using SqException::SqException;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_NotImplementedError_h_

