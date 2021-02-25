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

class OutOfRangeError
    : public Exception
{
public:
    using Exception::Exception;
    OutOfRangeError(const Token& token, std::string_view message);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_OutOfRangeError_h_
