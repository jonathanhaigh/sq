#include "common_types/Token.h"

#include "util/typeutil.h"

#include <iostream>

namespace sq {

namespace {

constexpr const char* token_kind_to_str(Token::Kind kind)
{
    switch (kind)
    {
        case Token::Kind::BoolTrue: return "BoolTrue";
        case Token::Kind::BoolFalse: return "BoolFalse";
        case Token::Kind::LParen: return "LParen";
        case Token::Kind::RParen: return "RParen";
        case Token::Kind::LBrace: return "LBrace";
        case Token::Kind::RBrace: return "RBrace";
        case Token::Kind::LBracket: return "LBracket";
        case Token::Kind::RBracket: return "RBracket";
        case Token::Kind::Dot: return "Dot";
        case Token::Kind::Equals: return "Equals";
        case Token::Kind::Comma: return "Comma";
        case Token::Kind::Colon: return "Colon";
        case Token::Kind::Identifier: return "Identifier";
        case Token::Kind::DQString: return "DQString";
        case Token::Kind::Integer: return "Integer";
        case Token::Kind::Eof: return "Eof";
    }
    return "UnknownTokenKind";
}

} // namespace

Token::Token(
    std::string_view query,
    std::ptrdiff_t pos,
    std::ptrdiff_t len,
    Kind kind
) noexcept
    : query_{query}
    , pos_{pos}
    , len_{len}
    , kind_{kind}
{ }

std::string_view Token::query() const noexcept
{
    return query_;
}

std::ptrdiff_t Token::pos() const noexcept
{
    return pos_;
}

std::ptrdiff_t Token::len() const noexcept
{
    return len_;
}

std::string_view Token::view() const noexcept
{
    return query_.substr(util::to_size_t(pos_), util::to_size_t(len_));
}

Token::Kind Token::kind() const noexcept
{
    return kind_;
}

std::ostream& operator<<(std::ostream& os, Token::Kind kind)
{
    os << token_kind_to_str(kind);
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    os << token.kind()
       << " token (" << token.view()
       << ") at position " << token.pos();
    return os;
}

} // namespace sq
