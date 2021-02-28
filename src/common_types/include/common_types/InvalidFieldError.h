/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_
#define SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_

#include "common_types/Exception.h"

namespace sq {

/**
 * Error indicating access of a non-existent field.
 */
class InvalidFieldError
    : public Exception
{
public:
    /**
     * @param sq_type the SQ type of the parent of the missing field.
     * @param field the name of the field that was requested.
     *
     * E.g. if the query is "a.b" and "b" is not a field of "a" then sq_type
     * should be the type of "a" and field should be "b".
     */
    InvalidFieldError(std::string_view sq_type, std::string_view field);
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_InvalidFieldError_h_
