/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/Token.h"

#include "util/typeutil.h"

#include <iostream>

namespace sq {

namespace {

constexpr std::string_view token_kind_to_str(TokenKind kind) {
  switch (kind) {
  case TokenKind::BoolFalse:
    return "BoolFalse";
  case TokenKind::BoolTrue:
    return "BoolTrue";
  case TokenKind::Colon:
    return "Colon";
  case TokenKind::Comma:
    return "Comma";
  case TokenKind::Dot:
    return "Dot";
  case TokenKind::DQString:
    return "DQString";
  case TokenKind::Eof:
    return "Eof";
  case TokenKind::Equals:
    return "Equals";
  case TokenKind::Float:
    return "Float";
  case TokenKind::GreaterThan:
    return "GreaterThan";
  case TokenKind::GreaterThanOrEqualTo:
    return "GreaterThanOrEqualTo";
  case TokenKind::Identifier:
    return "Identifier";
  case TokenKind::Integer:
    return "Integer";
  case TokenKind::LBrace:
    return "LBrace";
  case TokenKind::LBracket:
    return "LBracket";
  case TokenKind::LessThan:
    return "LessThan";
  case TokenKind::LessThanOrEqualTo:
    return "LessThanOrEqualTo";
  case TokenKind::LParen:
    return "LParen";
  case TokenKind::RBrace:
    return "RBrace";
  case TokenKind::RBracket:
    return "RBracket";
  case TokenKind::RParen:
    return "RParen";
  }
  return "UnknownTokenKind";
}

} // namespace

Token::Token(std::string_view query, gsl::index pos, gsl::index len,
             TokenKind kind) noexcept
    : query_{query}, pos_{pos}, len_{len}, kind_{kind} {}

std::string_view Token::query() const noexcept { return query_; }

gsl::index Token::pos() const noexcept { return pos_; }

gsl::index Token::len() const noexcept { return len_; }

std::string_view Token::view() const noexcept {
  return query_.substr(util::to_size(pos_), util::to_size(len_));
}

TokenKind Token::kind() const noexcept { return kind_; }

std::ostream &operator<<(std::ostream &os, TokenKind kind) {
  os << token_kind_to_str(kind);
  return os;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << token.kind() << " token (" << token.view() << ") at position "
     << token.pos();
  return os;
}

} // namespace sq
