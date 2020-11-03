#ifndef SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_
#define SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_

#include <stdexcept>

namespace sq {
namespace field_types {

class InvalidFieldError
    : public std::runtime_error
{
public:
    InvalidFieldError(std::string_view sq_type, std::string_view field);
};

} // namespace field_types
} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_
