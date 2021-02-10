#ifndef SQ_INCLUDE_GUARD_ast_ParseError_h_
#define SQ_INCLUDE_GUARD_ast_ParseError_h_

#include "common_types/SqException.h"

#include <string_view>

namespace antlr4 {
class Token;
} // namespace antlr4

namespace sq {

class SqParseError
    : public SqException
{
public:
    SqParseError(const antlr4::Token* token, std::string_view message);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_ast_ParseError_h_
