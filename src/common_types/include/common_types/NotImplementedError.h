/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_NotImplementedError_h_
#define SQ_INCLUDE_GUARD_common_types_NotImplementedError_h_

#include "common_types/Exception.h"

namespace sq {

/**
 * Error indicating that a requested feature has not been implemented.
 */
class NotImplementedError : public Exception {
public:
  using Exception::Exception;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_NotImplementedError_h_
