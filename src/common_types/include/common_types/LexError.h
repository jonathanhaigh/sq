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

class LexError
    : public ParseError
{
public:
    LexError(gsl::index pos, std::string_view query);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_LexError_h_
