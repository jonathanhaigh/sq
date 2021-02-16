#include "common_types/Exception.h"

namespace sq {

class OutOfRangeError
    : public Exception
{
    using Exception::Exception;
};

} // namespace sq
