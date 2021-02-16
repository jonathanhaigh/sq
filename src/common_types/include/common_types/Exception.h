#ifndef SQ_INCLUDE_GUARD_common_types_Exception_h_
#define SQ_INCLUDE_GUARD_common_types_Exception_h_

#include <stdexcept>

namespace sq {

class Exception
    : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Exception_h_
