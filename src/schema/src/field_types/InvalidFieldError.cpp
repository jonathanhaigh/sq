#include "field_types/InvalidFieldError.h"

#include <sstream>

namespace sq {
namespace field_types {

static std::string make_invalid_field_error_message(
    std::string_view sq_type,
    std::string_view field
)
{
    std::ostringstream os;
    os << sq_type << " has no field \"" << field << "\"";
    return os.str();
}

InvalidFieldError::InvalidFieldError(std::string_view sq_type, std::string_view field)
    : std::runtime_error{make_invalid_field_error_message(sq_type, field)}
{}

} // namespace field_types
} // namespace sq
