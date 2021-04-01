/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_
#define SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_

#include "core/ASSERT.h"
#include "core/typeutil.h"
#include "test/Primitive_test_util.h"

#include <range/v3/range/conversion.hpp>

namespace sq::test {

FakeField::FakeField(ResultGenerator result_generator,
                     PrimitiveLike auto &&primitive)
    : result_generator_{result_generator}, primitive_{test::to_primitive(
                                               SQ_FWD(primitive))} {}

FakeField::FakeField(PrimitiveLike auto &&primitive)
    : result_generator_{[=](const auto &, const auto &) {
        return std::make_shared<FakeField>(primitive);
      }},
      primitive_{test::to_primitive(SQ_FWD(primitive))} {}

FieldPtr fake_field(auto &&...args) {
  return std::make_shared<FakeField>(SQ_FWD(args)...);
}

void expect_field_accesses(MockField &mf, std::string_view field_name,
                           const FieldCallParams &params, Result &&retval,
                           auto &&...args) {
  expect_field_accesses(mf, field_name, params, std::move(retval));
  expect_field_accesses(mf, SQ_FWD(args)...);
}

StrictMockFieldPtr field_with_accesses(auto &&...args) {
  auto mf = std::make_shared<StrictMockField>();
  expect_field_accesses(*mf, SQ_FWD(args)...);
  return mf;
}

Result to_field_range(ranges::category cat, ranges::cpp20::view auto &&rng) {
  if (cat == input) {
    return FieldRange<input>{SQ_FWD(rng)};
  }
  if (cat == (input | sized)) {
    return FieldRange<input | sized>{SQ_FWD(rng)};
  }
  if (cat == forward) {
    return FieldRange<forward>{SQ_FWD(rng)};
  }
  if (cat == (forward | sized)) {
    return FieldRange<forward | sized>{SQ_FWD(rng)};
  }
  if (cat == bidirectional) {
    return FieldRange<bidirectional>{SQ_FWD(rng)};
  }
  if (cat == (bidirectional | sized)) {
    return FieldRange<bidirectional | sized>{SQ_FWD(rng)};
  }
  if (cat == random_access) {
    return FieldRange<random_access>{SQ_FWD(rng)};
  }
  if (cat == (random_access | sized)) {
    return FieldRange<random_access | sized>{SQ_FWD(rng)};
  }
  ASSERT(false);
  return FieldRange<input>{SQ_FWD(rng)};
}

} // namespace sq::test

#endif // SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_
