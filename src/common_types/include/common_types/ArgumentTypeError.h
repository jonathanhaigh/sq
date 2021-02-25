/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_ArgumentTypeError_h_
#define SQ_INCLUDE_GUARD_common_types_ArgumentTypeError_h_

#include "common_types/Exception.h"
#include "common_types/Primitive.h"

#include <string_view>

namespace sq {

class ArgumentTypeError
    : public Exception
{
public:
    ArgumentTypeError(const Primitive& received, std::string_view type_expected);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_ArgumentTypeError_h_
