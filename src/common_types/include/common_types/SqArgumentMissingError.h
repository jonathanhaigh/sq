#ifndef SQ_INCLUDE_GUARD_common_types_SqArgumentMissingError_h_
#define SQ_INCLUDE_GUARD_common_types_SqArgumentMissingError_h_

#include "common_types/SqException.h"

#include <string_view>

namespace sq {

class SqArgumentMissingError
    : public SqException
{
public:
    SqArgumentMissingError(std::string_view arg_name, std::string_view arg_type);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_SqArgumentMissingError_h_
