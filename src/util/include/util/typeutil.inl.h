/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_util_typeutil_inl_h_
#define SQ_INCLUDE_GUARD_util_typeutil_inl_h_

#include <typeinfo>

namespace sq::util {

namespace detail {

SQ_ND std::string demangle(const char *type_name);

} // namespace detail

SQ_ND std::string base_type_name(const auto &thing) {
  return detail::demangle(typeid(thing).name());
}

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_typeutil_inl_h_
