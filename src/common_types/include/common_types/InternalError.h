/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_field_types_InternalError_h_
#define SQ_INCLUDE_GUARD_field_types_InternalError_h_

#include "common_types/Exception.h"

namespace sq {

class InternalError
    : public Exception
{
public:
    using Exception::Exception;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_InternalError_h_

