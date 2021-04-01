/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "test/results_test_util.h"

#include "results/Serializer.h"

#include <rapidjson/document.h>

namespace sq::test {

FakeField::FakeField(Result &&result)
    : result_generator_{
          [r = std::move(result)](const auto &, const auto &) { return r; }} {}

FakeField::FakeField(ResultGenerator result_generator)
    : result_generator_{result_generator} {}

FakeField::FakeField() : FakeField{PrimitiveInt{0}} {}

Result FakeField::get(std::string_view member,
                      const FieldCallParams &params) const {
  return result_generator_(member, params);
}

Primitive FakeField::to_primitive() const { return primitive_; }

Result fake_field_range(PrimitiveInt start, PrimitiveInt stop,
                        ranges::category cat) {
  return to_field_range(cat, rv::iota(start, stop) | rv::transform([](auto i) {
                               return std::make_shared<FakeField>(i);
                             }));
}

void expect_field_accesses(MockField &mf, std::string_view field_name,
                           const FieldCallParams &params, Result &&retval) {
  EXPECT_CALL(mf, get(field_name, params))
      .Times(1)
      .WillOnce(Return(ByMove(std::move(retval))));
}

StrictMockFieldPtr field_with_no_accesses() {
  return std::make_shared<StrictMockField>();
}

void expect_equivalent_json(std::string_view json1, std::string_view json2) {
  SCOPED_TRACE(testing::Message()
               << "assert_equivalent_json(" << json1 << ", " << json2 << ")");
  ::rapidjson::Document d1{};
  d1.Parse(json1.data(), json1.size());
  ::rapidjson::Document d2{};
  d2.Parse(json2.data(), json2.size());
  EXPECT_EQ(d1, d2);
}

} // namespace sq::test

namespace ranges {

std::ostream &operator<<(std::ostream &os, const category &cat) {
  if ((cat & category::sized) == category::sized) {
    os << "sized ";
  }
  if ((cat & category::mask) == category::random_access) {
    os << "random_access";
  } else if ((cat & category::mask) == category::bidirectional) {
    os << "bidirectional";
  } else if ((cat & category::mask) == category::forward) {
    os << "forward";
  } else if ((cat & category::mask) == category::input) {
    os << "input";
  } else {
    os << "unknown";
  }
  return os;
}

} // namespace ranges
