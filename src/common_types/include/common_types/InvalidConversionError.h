/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_InvalidConversionError_
#define SQ_INCLUDE_GUARD_common_types_InvalidConversionError_

#include "common_types/Exception.h"

#include <string_view>

namespace sq {

class InvalidConversionError
    : public Exception
{
public:
    using Exception::Exception;
    InvalidConversionError(std::string_view from, std::string_view to);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_InvalidConversionError_
