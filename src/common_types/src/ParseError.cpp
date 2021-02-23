#include "common_types/ParseError.h"

#include "util/strutil.h"
#include "util/typeutil.h"

#include <sstream>

namespace sq {

namespace {

[[nodiscard]] std::string create_parse_error_message(
    const Token& token,
    const Token::KindSet& expecting
)
{
    auto ss = std::ostringstream{};
    ss << "parse error: unexpected " << token
       << "; Expecting one of: " << util::join(expecting)
       << token.query()
       << "\n" << std::string(util::to_size_t(token.pos()), ' ') << "^\n";
    return ss.str();
}

} // namespace

ParseError::ParseError(const Token& token, const Token::KindSet& expecting)
    : Exception{create_parse_error_message(token, expecting)}
{ }

} // namespace sq
