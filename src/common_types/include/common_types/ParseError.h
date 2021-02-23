#ifndef SQ_INCLUDE_GUARD_common_types_ParseError_h_
#define SQ_INCLUDE_GUARD_common_types_ParseError_h_

#include "common_types/Exception.h"
#include "common_types/Token.h"

#include <cstddef>
#include <string_view>

namespace sq {

class ParseError
    : public Exception
{
public:
    using Exception::Exception;
    ParseError(const Token& token, const Token::KindSet& expecting);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_ParseError_h_
