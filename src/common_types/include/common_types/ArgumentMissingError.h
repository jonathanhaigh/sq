#ifndef SQ_INCLUDE_GUARD_common_types_ArgumentMissingError_h_
#define SQ_INCLUDE_GUARD_common_types_ArgumentMissingError_h_

#include "common_types/Exception.h"

#include <string_view>

namespace sq {

class ArgumentMissingError
    : public Exception
{
public:
    ArgumentMissingError(std::string_view arg_name, std::string_view arg_type);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_ArgumentMissingError_h_
