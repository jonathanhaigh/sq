#ifndef SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_
#define SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_

#include "exceptions/SqException.h"

namespace sq {

class InvalidFieldError
    : public SqException
{
public:
    InvalidFieldError(std::string_view sq_type, std::string_view field);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_
