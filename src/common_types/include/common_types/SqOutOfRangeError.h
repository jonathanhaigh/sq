#include "common_types/SqException.h"

namespace sq {

class SqOutOfRangeError
    : public SqException
{
    using SqException::SqException;
};

} // namespace sq
