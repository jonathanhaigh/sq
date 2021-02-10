#include "common_types/SqArgumentMissingError.h"

#include <sstream>

namespace sq {

static std::string create_error_message(
    const std::string_view arg_name,
    const std::string_view arg_type
)
{
    auto ss = std::ostringstream{};
    ss << "Missing argument " << arg_name
       << " of type " << arg_type;
    return ss.str();
}

SqArgumentMissingError::SqArgumentMissingError(
    const std::string_view arg_name,
    const std::string_view arg_type
)
    : SqException(create_error_message(arg_name, arg_type))
{ }

} // namespace sq

