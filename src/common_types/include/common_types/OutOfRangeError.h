/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_OutOfRangeError_h_
#define SQ_INCLUDE_GUARD_common_types_OutOfRangeError_h_

#include "common_types/Exception.h"

#include <string_view>

namespace sq {

class Token;

/**
 * Error indicating a request to access an element outside of an allowed range.
 */
class OutOfRangeError
    : public Exception
{
public:
    using Exception::Exception;
    /**
     * @param token the token in the query where the access was requested.
     * @param message details about the requested access.
     */
    OutOfRangeError(const Token& token, std::string_view message);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_OutOfRangeError_h_
