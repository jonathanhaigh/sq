/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_typeutil_inl_h_
#define SQ_INCLUDE_GUARD_core_typeutil_inl_h_

#include "core/errors.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <typeinfo>

namespace sq {

namespace detail {

SQ_ND std::string demangle(const char *type_name);

} // namespace detail

SQ_ND std::string base_type_name(const auto &thing) {
  return detail::demangle(typeid(thing).name());
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_core_typeutil_inl_h_
