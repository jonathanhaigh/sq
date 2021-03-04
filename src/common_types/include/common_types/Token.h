/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Token_h_
#define SQ_INCLUDE_GUARD_common_types_Token_h_

#include "util/typeutil.h"

#include <gsl/gsl>
#include <iosfwd>
#include <regex>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace sq {

/**
 * Represents a single token of an SQ query.
 *
 * Token objects do not own the string of characters that make up the token
 * text. I.e. they contain a reference to the characters in the original query
 * string and that reference is only valid as long as the original query string
 * is alive.
 */
class Token
{
public:
    /**
     * Kind of token.
     */
    enum class Kind
    {
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

    /**
     * A set of token kinds.
     */
    using KindSet = std::unordered_set<Kind>;

    /**
     * Create a Token object.
     *
     * @param query the full query string in which the token was found.
     * @param pos the character position within the query at which the token
     *        was found.
     * @param len the length, in characters, of the token.
     * @param kind the kind of the token.
     */
    Token(
        std::string_view query,
        gsl::index pos,
        gsl::index len,
        Kind kind
    ) noexcept;

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
    SQ_ND Kind kind() const noexcept;

private:
    std::string_view query_;
    gsl::index pos_;
    gsl::index len_;
    Kind kind_;
};

std::ostream& operator<<(std::ostream& os, Token::Kind kind);

/**
 * Print information about a token.
 *
 * The text printed includes information about the position of the token in the
 * input query, not just the characters that make up the token.
 */
std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Token_h_
