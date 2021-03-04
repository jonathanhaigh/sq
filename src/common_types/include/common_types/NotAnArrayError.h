/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_NotAnArrayError_h_
#define SQ_INCLUDE_GUARD_common_types_NotAnArrayError_h_

#include "common_types/Exception.h"

namespace sq {

/**
 * Error indicating that an array operation has been requested on a non-array
 * type.
 */
class NotAnArrayError : public Exception {
public:
  using Exception::Exception;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_NotAnArrayError_h_
