#ifndef SQ_INCLUDE_GUARD_exceptions_NotImplementedError_h_
#define SQ_INCLUDE_GUARD_exceptions_NotImplementedError_h_

#include "exceptions/SqException.h"

namespace sq {

class NotImplementedError
    : public SqException
{
public:
    using SqException::SqException;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_exceptions_NotImplementedError_h_

