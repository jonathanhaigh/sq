/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_Token_fwd_h_
#define SQ_INCLUDE_GUARD_core_Token_fwd_h_

#include <unordered_set>

namespace sq {

/**
 * Kind of token.
 */
enum class TokenKind : int;

/**
 * A set of token kinds.
 */
using TokenKindSet = std::unordered_set<TokenKind>;

/**
 * Represents a single token of an SQ query.
 *
 * Token objects do not own the string of characters that make up the token
 * text. I.e. they contain a reference to the characters in the original query
 * string and that reference is only valid as long as the original query string
 * is alive.
 */
class Token;

} // namespace sq

#endif // SQ_INCLUDE_GUARD_core_Token_fwd_h_
