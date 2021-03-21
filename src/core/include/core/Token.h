/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_Token_h_
#define SQ_INCLUDE_GUARD_core_Token_h_

#include "Token.fwd.h"

#include "core/typeutil.h"

#include <gsl/gsl>
#include <iosfwd>
#include <regex>
#include <string_view>
#include <vector>

namespace sq {

enum class TokenKind : int {
  BoolFalse,
  BoolTrue,
  Colon,
  Comma,
  Dot,
  DQString,
  Eof,
  Equals,
  Float,
  GreaterThan,
  GreaterThanOrEqualTo,
  Identifier,
  Integer,
  LBrace,
  LBracket,
  LessThan,
  LessThanOrEqualTo,
  LParen,
  RBrace,
  RBracket,
  RParen
};

class Token {
public:
  /**
   * Create a Token object.
   *
   * @param query the full query string in which the token was found.
   * @param pos the character position within the query at which the token
   *        was found.
   * @param len the length, in characters, of the token.
   * @param kind the kind of the token.
   */
  Token(std::string_view query, gsl::index pos, gsl::index len,
        TokenKind kind) noexcept;

  /**
   * Get the full query string in which the token was found.
   */
  SQ_ND std::string_view query() const noexcept;

  /**
   * Get the character position within the query at which the token was
   * found.
   */
  SQ_ND gsl::index pos() const noexcept;

  /**
   * Get the length, in characters, of the token.
   */
  SQ_ND gsl::index len() const noexcept;

  /**
   * Get a std::string_view pointing to the characters of the token.
   */
  SQ_ND std::string_view view() const noexcept;

  /**
   * Get the kind of the token.
   */
  SQ_ND TokenKind kind() const noexcept;

private:
  std::string_view query_;
  gsl::index pos_;
  gsl::index len_;
  TokenKind kind_;
};

std::ostream &operator<<(std::ostream &os, TokenKind kind);

/**
 * Print information about a token.
 *
 * The text printed includes information about the position of the token in the
 * input query, not just the characters that make up the token.
 */
std::ostream &operator<<(std::ostream &os, const Token &token);

} // namespace sq

#endif // SQ_INCLUDE_GUARD_core_Token_h_
