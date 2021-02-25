/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Token_h_
#define SQ_INCLUDE_GUARD_common_types_Token_h_

#include <cstddef>
#include <iosfwd>
#include <regex>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace sq {

class Token
{
public:
    enum class Kind
    {
        BoolTrue,
        BoolFalse,
        LParen,
        RParen,
        LBrace,
        RBrace,
        LBracket,
        RBracket,
        Dot,
        Equals,
        Comma,
        Colon,
        Identifier,
        DQString,
        Integer,
        Eof
    };

    using KindSet = std::unordered_set<Kind>;

    Token(
        std::string_view query,
        std::ptrdiff_t pos,
        std::ptrdiff_t len,
        Kind kind
    ) noexcept;

    [[nodiscard]] std::string_view query() const noexcept;
    [[nodiscard]] std::ptrdiff_t pos() const noexcept;
    [[nodiscard]] std::ptrdiff_t len() const noexcept;
    [[nodiscard]] std::string_view view() const noexcept;
    [[nodiscard]] Kind kind() const noexcept;

private:
    std::string_view query_;
    std::ptrdiff_t pos_;
    std::ptrdiff_t len_;
    Kind kind_;
};

std::ostream& operator<<(std::ostream& os, Token::Kind kind);
std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Token_h_
