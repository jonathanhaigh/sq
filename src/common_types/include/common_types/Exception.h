/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Exception_h_
#define SQ_INCLUDE_GUARD_common_types_Exception_h_

#include <stdexcept>

namespace sq {

/**
 * Base class for errors thrown by the SQ code.
 */
class Exception
    : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Exception_h_
