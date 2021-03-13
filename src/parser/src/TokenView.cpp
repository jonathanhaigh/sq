/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "parser/TokenView.h"

#include "common_types/errors.h"
#include "util/ASSERT.h"
#include "util/typeutil.h"

#include <gsl/gsl>

namespace sq::parser {

namespace {

const auto &token_regex_map() {
  static const auto map = std::array{
      std::pair{TokenKind::LParen, std::regex{"[(]"}},
      std::pair{TokenKind::RParen, std::regex{"[)]"}},
      std::pair{TokenKind::LBrace, std::regex{"[{]"}},
      std::pair{TokenKind::RBrace, std::regex{"[}]"}},
      std::pair{TokenKind::LBracket, std::regex{"\\["}},
      std::pair{TokenKind::RBracket, std::regex{"\\]"}},
      std::pair{TokenKind::Comma, std::regex{","}},
      std::pair{TokenKind::Colon, std::regex{":"}},
      std::pair{TokenKind::DQString, std::regex{R"%("(?:[^"\\]|\\.)*")%"}},

      // Order matters here:
      // * Prefer to match "<=" than "<" then "=".
      // * Prefer to match ">=" than ">" then "=".
      std::pair{TokenKind::LessThanOrEqualTo, std::regex{"<="}},
      std::pair{TokenKind::LessThan, std::regex{"<"}},
      std::pair{TokenKind::GreaterThanOrEqualTo, std::regex{">="}},
      std::pair{TokenKind::GreaterThan, std::regex{">"}},
      std::pair{TokenKind::Equals, std::regex{"="}},

      // Order matters here:
      // * Prefer to match "true" and "false" before identifiers but only if
      //   it doesn't look like "true" or "false" is just the start of a
      //   longer identifier (e.g. "true1", "false_id")
      std::pair{TokenKind::BoolTrue, std::regex{"true(?![A-Za-z_0-9])"}},
      std::pair{TokenKind::BoolFalse, std::regex{"false(?![A-Za-z_0-9])"}},
      std::pair{TokenKind::Identifier, std::regex{"[A-Za-z_][A-Za-z_0-9]*"}},

      // Note that order matters here:
      // * Prefer to match an Integer to a Float, but only if there's no
      //   "." after the integer.
      // * Prefer to match a Float to a Dot.
      std::pair{TokenKind::Integer, std::regex{"-?[0-9]+(?![0-9.])"}},
      std::pair{
          TokenKind::Float,
          std::regex{
              "[+-]?(?=[.]?[0-9])[0-9]*(?:[.][0-9]*)?(?:[Ee][+-]?[0-9]+)?"}},
      std::pair{TokenKind::Dot, std::regex{"[.]"}}};
  return map;
}

} // namespace

TokenView::TokenView(std::string_view str) noexcept : str_{str} {}

const Token &TokenView::read() const {
  Expects(pos_ != -1);

  if (cache_) {
    return cache_.value();
  }

  const auto pos = pos_ + whitespace_length();
  const auto remaining = str_.substr(util::to_size(pos));

  if (std::ssize(remaining) == 0) {
    cache_ = Token(str_, pos, 0, TokenKind::Eof);
    return cache_.value();
  }

  for (const auto &[token_kind, regex] : token_regex_map()) {
    if (auto match = std::match_results<std::string_view::const_iterator>{};
        std::regex_search(remaining.begin(), remaining.end(), match, regex,
                          std::regex_constants::match_continuous)) {
      cache_ = Token(str_, pos, match.length(), token_kind);
      return cache_.value();
    }
  }

  throw LexError{pos, str_};
}

void TokenView::next() {
  if (!cache_) {
    (void)read();
    ASSERT(cache_);
  }
  if (cache_.value().kind() == TokenKind::Eof) {
    pos_ = -1;
    cache_ = std::nullopt;
    return;
  }
  pos_ = cache_.value().pos() + cache_.value().len();
  cache_ = std::nullopt;
}

gsl::index TokenView::whitespace_length() const {
  const auto remaining = str_.substr(util::to_size(pos_));
  return util::to_index(
      std::min(remaining.find_first_not_of(" \t\r\n"), remaining.size()));
}

bool TokenView::equal(SQ_MU ranges::default_sentinel_t other) const noexcept {
  return pos_ == -1;
}

} // namespace sq::parser
