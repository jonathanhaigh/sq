/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Token_h_
#define SQ_INCLUDE_GUARD_common_types_Token_h_

#include <gsl/gsl>
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
        gsl::index pos,
        gsl::index len,
        Kind kind
    ) noexcept;

    [[nodiscard]] std::string_view query() const noexcept;
    [[nodiscard]] gsl::index pos() const noexcept;
    [[nodiscard]] gsl::index len() const noexcept;
    [[nodiscard]] std::string_view view() const noexcept;
    [[nodiscard]] Kind kind() const noexcept;

private:
    std::string_view query_;
    gsl::index pos_;
    gsl::index len_;
    Kind kind_;
};

std::ostream& operator<<(std::ostream& os, Token::Kind kind);
std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Token_h_
