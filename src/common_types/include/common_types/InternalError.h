/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_field_types_InternalError_h_
#define SQ_INCLUDE_GUARD_field_types_InternalError_h_

#include "common_types/Exception.h"

namespace sq {

/**
 * Exception indicating a programming error in SQ.
 *
 * InternalError should never actually be thrown - they are used in places that
 * the programmer believes are dead code, but where the C++ language still
 * requires e.g. a return statement.
 */
class InternalError : public Exception {
public:
  using Exception::Exception;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_InternalError_h_
