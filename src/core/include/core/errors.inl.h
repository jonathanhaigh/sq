/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_errors_inl_h_
#define SQ_INCLUDE_GUARD_core_errors_inl_h_

namespace sq {

NarrowingError::NarrowingError(auto &&target, auto &&value,
                               auto &&kind_of_value_format,
                               auto &&...format_args)
    : OutOfRangeError{fmt::format(
          "{} ({}) does not fit in type {}",
          fmt::format(SQ_FWD(kind_of_value_format), SQ_FWD(format_args)...),
          SQ_FWD(value), base_type_name(SQ_FWD(target)))} {}

NarrowingError::NarrowingError(auto &&target, auto &&value)
    : NarrowingError{SQ_FWD(target), SQ_FWD(value),
                     base_type_name(SQ_FWD(value))} {}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_core_errors_inl_h_
