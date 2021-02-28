/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_ArgumentMissingError_h_
#define SQ_INCLUDE_GUARD_common_types_ArgumentMissingError_h_

#include "common_types/Exception.h"

#include <string_view>

namespace sq {


/**
 * Error indicating that a required parameter of a field is missing.
 */
class ArgumentMissingError
    : public Exception
{
public:
    /**
     * @param arg_name the name of the missing argument.
     * @param arg_type the type of the missing argument.
     */
    ArgumentMissingError(std::string_view arg_name, std::string_view arg_type);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_ArgumentMissingError_h_
