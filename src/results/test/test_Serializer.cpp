/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/Serializer.h"

#include "results/results.h"
#include "test/results_test_util.h"

#include <cctype>
#include <gtest/gtest.h>
#include <range/v3/action/remove_if.hpp>
#include <string_view>

namespace sq::test {

inline bool isspace(char c) {
  return static_cast<bool>(std::isspace(static_cast<unsigned char>(c)));
}

struct SerializerTest : public ::testing::Test {
  SerializerTest() : serializer{results::get_serializer(ostream)} {}
  std::ostringstream ostream;
  std::unique_ptr<results::Serializer> serializer;
};

struct SerializePrimitiveTestCase {
  SerializePrimitiveTestCase(const char *expected, const Primitive &value)
      : expected{expected}, value{value} {}

  const char *expected;
  Primitive value;
};

std::ostream &operator<<(std::ostream &os,
                         const SerializePrimitiveTestCase &tc) {
  os << tc.expected;
  return os;
}

struct SerializePrimitiveTest
    : SerializerTest,
      ::testing::WithParamInterface<SerializePrimitiveTestCase> {};

TEST_P(SerializePrimitiveTest, TestSerializePrimitive) {
  const auto tc = GetParam();
  serializer->write_value(tc.value);
  expect_equivalent_json(ostream.str(), tc.expected);
}

INSTANTIATE_TEST_SUITE_P(
    Primitives, SerializePrimitiveTest,
    ::testing::Values(
        SerializePrimitiveTestCase{"1", PrimitiveInt{1}},
        SerializePrimitiveTestCase{"-1", PrimitiveInt{-1}},
        SerializePrimitiveTestCase{"99", PrimitiveInt{99}},
        SerializePrimitiveTestCase{"-99", PrimitiveInt{-99}},
        SerializePrimitiveTestCase{"1.2", PrimitiveFloat{1.2}},
        SerializePrimitiveTestCase{"-1.2", PrimitiveFloat{-1.2}},
        SerializePrimitiveTestCase{"99.2", PrimitiveFloat{99.2}},
        SerializePrimitiveTestCase{"-99.2", PrimitiveFloat{-99.2}},
        SerializePrimitiveTestCase{"1.0", PrimitiveFloat{1.0}},
        SerializePrimitiveTestCase{"-1.0", PrimitiveFloat{-1.0}},
        SerializePrimitiveTestCase{"99.0", PrimitiveFloat{99.0}},
        SerializePrimitiveTestCase{"-99.0", PrimitiveFloat{-99.0}},
        SerializePrimitiveTestCase{R"("")", PrimitiveString{""}},
        SerializePrimitiveTestCase{R"("str")", PrimitiveString{"str"}},
        SerializePrimitiveTestCase{"true", PrimitiveBool{true}},
        SerializePrimitiveTestCase{"false", PrimitiveBool{false}}));

TEST_F(SerializerTest, TestSerializeObject) {
  serializer->start_object();
  EXPECT_EQ(ostream.str(), "{");
  serializer->write_key("a");
  serializer->write_value(PrimitiveInt{0});
  serializer->write_key("b");
  serializer->write_value(PrimitiveInt{1});
  serializer->end_object();
  expect_equivalent_json(ostream.str(), R"({"a": 0, "b": 1})");
}

TEST_F(SerializerTest, TestSerializeArray) {
  serializer->start_array();
  EXPECT_EQ(ostream.str(), "[");
  serializer->write_value(PrimitiveInt{0});
  serializer->write_value(PrimitiveInt{1});
  serializer->end_array();
  expect_equivalent_json(ostream.str(), "[0, 1]");
}

} // namespace sq::test
