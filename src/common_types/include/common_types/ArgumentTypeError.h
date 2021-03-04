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

/**
 * Error indicating that a given parameter is of incorrect type.
 */
class ArgumentTypeError : public Exception {
public:
  /**
   * @param received the value of the given parameter.
   * @param type_expected the name of the type expected for the parameter.
   */
  ArgumentTypeError(const Primitive &received, std::string_view type_expected);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_ArgumentTypeError_h_
