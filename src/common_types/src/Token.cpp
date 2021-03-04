/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/Token.h"

#include "util/typeutil.h"

#include <iostream>

namespace sq {

namespace {

constexpr std::string_view token_kind_to_str(Token::Kind kind) {
  switch (kind) {
  case Token::Kind::BoolFalse:
    return "BoolFalse";
  case Token::Kind::BoolTrue:
    return "BoolTrue";
  case Token::Kind::Colon:
    return "Colon";
  case Token::Kind::Comma:
    return "Comma";
  case Token::Kind::Dot:
    return "Dot";
  case Token::Kind::DQString:
    return "DQString";
  case Token::Kind::Eof:
    return "Eof";
  case Token::Kind::Equals:
    return "Equals";
  case Token::Kind::Float:
    return "Float";
  case Token::Kind::GreaterThan:
    return "GreaterThan";
  case Token::Kind::GreaterThanOrEqualTo:
    return "GreaterThanOrEqualTo";
  case Token::Kind::Identifier:
    return "Identifier";
  case Token::Kind::Integer:
    return "Integer";
  case Token::Kind::LBrace:
    return "LBrace";
  case Token::Kind::LBracket:
    return "LBracket";
  case Token::Kind::LessThan:
    return "LessThan";
  case Token::Kind::LessThanOrEqualTo:
    return "LessThanOrEqualTo";
  case Token::Kind::LParen:
    return "LParen";
  case Token::Kind::RBrace:
    return "RBrace";
  case Token::Kind::RBracket:
    return "RBracket";
  case Token::Kind::RParen:
    return "RParen";
  }
  return "UnknownTokenKind";
}

} // namespace

Token::Token(std::string_view query, gsl::index pos, gsl::index len,
             Kind kind) noexcept
    : query_{query}, pos_{pos}, len_{len}, kind_{kind} {}

std::string_view Token::query() const noexcept { return query_; }

gsl::index Token::pos() const noexcept { return pos_; }

gsl::index Token::len() const noexcept { return len_; }

std::string_view Token::view() const noexcept {
  return query_.substr(util::to_size(pos_), util::to_size(len_));
}

Token::Kind Token::kind() const noexcept { return kind_; }

std::ostream &operator<<(std::ostream &os, Token::Kind kind) {
  os << token_kind_to_str(kind);
  return os;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << token.kind() << " token (" << token.view() << ") at position "
     << token.pos();
  return os;
}

} // namespace sq
