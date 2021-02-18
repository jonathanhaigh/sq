#include "ast/ParseError.h"

#include "parser/SqParser.h"

#include <sstream>

namespace sq {

namespace {

[[nodiscard]] std::string create_parse_error_message(
    const antlr4::Token* const token,
    std::string_view message
)
{
    auto ss = std::ostringstream{};
    if (token == nullptr)
    {
        ss << "Parse error at unknown location";
    }
    else
    {
        ss << "Parse error at line " << token->getLine()
           << " position " << token->getStartIndex();
    }
    ss << ": " << message;
    return ss.str();
}

} // namespace

ParseError::ParseError(
    const antlr4::Token* const token,
    std::string_view message
)
    : Exception{create_parse_error_message(token, message)}
{ }

} // namespace sq
