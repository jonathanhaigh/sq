/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_LexError_h_
#define SQ_INCLUDE_GUARD_common_types_LexError_h_

#include "common_types/ParseError.h"

#include <gsl/gsl>
#include <string_view>

namespace sq {

/**
 * Indicates a failure to interpret part of the input query as a token.
 */
class LexError : public ParseError {
public:
  /**
   * @param pos position in the input query (in characters) at which the lex
   *      error occured.
   * @param query the full input query.
   */
  LexError(gsl::index pos, std::string_view query);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_LexError_h_
