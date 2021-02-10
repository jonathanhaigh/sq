#include "common_types/SqArgumentTypeError.h"

#include <sstream>

namespace sq {

static std::string create_error_message(
    const Primitive& received,
    const std::string_view type_expected
)
{
    auto ss = std::ostringstream{};
    ss << "Invalid argument " << primitive_to_str(received)
       << " of type " << primitive_type_name(received)
       << " expecting " << type_expected;
    return ss.str();
}

SqArgumentTypeError::SqArgumentTypeError(
    const Primitive& received,
    const std::string_view type_expected
)
    : SqException(create_error_message(received, type_expected))
{ }

} // namespace sq
