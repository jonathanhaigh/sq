/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_test_FieldCallParams_test_util_inl_h_
#define SQ_INCLUDE_GUARD_core_test_FieldCallParams_test_util_inl_h_

#include "core/typeutil.h"
#include "test/Primitive_test_util.h"

#include <string_view>
#include <utility>

namespace sq::test {

namespace detail {

void add_params(FieldCallParams &fcp, PrimitiveLike auto &&p) {
  fcp.pos_params().emplace_back(to_primitive(SQ_FWD(p)));
}

inline void add_params(FieldCallParams &fcp, NamedParam &&p) {
  fcp.named_params().emplace(std::move(p));
}

void add_params(FieldCallParams &fct, auto &&p, auto &&...args) {
  add_params(fct, SQ_FWD(p));
  add_params(fct, SQ_FWD(args)...);
}

} // namespace detail

FieldCallParams params(auto &&...args) {
  auto ret = FieldCallParams{};
  detail::add_params(ret, SQ_FWD(args)...);
  return ret;
}

NamedParam named(std::string_view name, PrimitiveLike auto &&np) {
  return NamedParam{name, to_primitive(SQ_FWD(np))};
}

} // namespace sq::test

#endif // SQ_INCLUDE_GUARD_core_test_FieldCallParams_test_util_inl_h_
