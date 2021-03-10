/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_field_types_PullupWithSiblingsError_h_
#define SQ_INCLUDE_GUARD_field_types_PullupWithSiblingsError_h_

#include "common_types/Exception.h"

namespace sq {

/**
 * Error indicating that a pullup type field access has been requested for a
 * field access with siblings.
 */
class PullupWithSiblingsError : public Exception {
  using Exception::Exception;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_PullupWithSiblingsError_h_
