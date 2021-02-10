#include "common_types/InvalidFieldError.h"

#include <sstream>

namespace sq {

static std::string make_invalid_field_error_message(
    const std::string_view sq_type,
    const std::string_view field
)
{
    auto os = std::ostringstream{};
    os << sq_type << " has no field \"" << field << "\"";
    return os.str();
}

InvalidFieldError::InvalidFieldError(const std::string_view sq_type, const std::string_view field)
    : SqException{make_invalid_field_error_message(sq_type, field)}
{}

} // namespace sq
