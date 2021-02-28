/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_ParseError_h_
#define SQ_INCLUDE_GUARD_common_types_ParseError_h_

#include "common_types/Exception.h"
#include "common_types/Token.h"

#include <cstddef>
#include <string_view>

namespace sq {

/**
 * Error indicating incorrect grammar in a query.
 */
class ParseError
    : public Exception
{
public:
    using Exception::Exception;

    /**
     * Create a ParseError for when an unexpected token is found.
     *
     * @param token the unexpected token.
     * @param expecting the set of tokens that would have been valid in place
     * of the unexpected token.
     */
    ParseError(const Token& token, const Token::KindSet& expecting);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_ParseError_h_
