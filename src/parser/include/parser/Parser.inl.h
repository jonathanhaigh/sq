#ifndef SQ_INCLUDE_GUARD_parser_Parser_inl_h_
#define SQ_INCLUDE_GUARD_parser_Parser_inl_h_

#include <charconv>

#include "common_types/OutOfRangeError.h"
#include "util/ASSERT.h"

namespace sq::parser {

template <std::integral Int>
std::optional<Int> Parser::parse_integer()
{
    const auto opt_token = accept_token(Token::Kind::Integer);
    if (!opt_token)
    {
        return std::nullopt;
    }
    const auto& token = opt_token.value();
    const auto str_view = token.view();
    const auto* const begin = str_view.data();
    // We can't really avoid using pointer arithmetic when using
    // std::from_chars - it requires a const char* to indicate the end of the
    // string, but we can only reasonably make one using pointer arithmetic.
    // In particular, std::string, std::string_view, std::span etc. only
    // provide iterator versions of end(), but we need a pointer.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    const auto* const end = begin + str_view.size();
    Int value = 0;
    const auto [ptr, ec] = std::from_chars(begin, end, value, 10);
    if (ec == std::errc::result_out_of_range)
    {
        auto ss = std::ostringstream{};
        ss << "integer " << str_view << " does not fit in required type; "
           << "must be in the closed interval ["
           << std::numeric_limits<Int>::min() << ", "
           << std::numeric_limits<Int>::max() << "]";
        throw OutOfRangeError(token, ss.str());
    }
    ASSERT(ec == std::errc{});
    ASSERT(ptr == end);
    return value;
}

} // namespace sq::parser

#endif // SQ_INCLUDE_GUARD_parser_Parser_inl_h_
