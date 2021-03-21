/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/results.h"

#include "core/errors.h"
#include "core/narrow.h"
#include "core/strutil.h"
#include "core/typeutil.h"
#include "parser/Ast.h"
#include "parser/Parser.h"
#include "parser/TokenView.h"
#include "test/FieldCallParams_test_util.h"
#include "test/results_test_util.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gsl/gsl>
#include <gtest/gtest.h>
#include <iostream>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <utility>

namespace sq::test {
namespace {

using namespace sq::results;

inline constexpr auto all_comparison_ops = {
    parser::ComparisonOperator::GreaterThanOrEqualTo,
    parser::ComparisonOperator::GreaterThan,
    parser::ComparisonOperator::LessThanOrEqualTo,
    parser::ComparisonOperator::LessThan, parser::ComparisonOperator::Equals};

parser::Ast generate_ast(std::string_view query) {
  auto tokens = parser::TokenView{query};
  auto parser = parser::Parser{tokens};
  return parser.parse();
}

struct ResultTreeTest : public MockFieldTest {};

// -----------------------------------------------------------------------------
// Minimal system call tests:
// Ensure that the results gathering library doesn't call into the system more
// than it needs to. The results gathering code should assume that all calls
// into the system are expensive and should cache results if repeated access is
// required.
// -----------------------------------------------------------------------------
TEST_F(ResultTreeTest, TestMinimalSystemCallsWithSingleCallPerObject) {
  const auto ast = generate_ast("a.b.c");
  const auto fcp = FieldCallParams{};
  auto c = field_with_no_accesses();
  auto b = field_with_accesses("c", fcp, std::move(c));
  auto a = field_with_accesses("b", fcp, std::move(b));
  auto root = field_with_accesses("a", fcp, std::move(a));
  const auto results = ResultTree(ast, std::move(root));
}

TEST_F(ResultTreeTest, TestMinimalSystemCallsWithMultipleCallsPerObject) {
  const auto ast = generate_ast("a { b c d }");
  const auto fcp = FieldCallParams{};
  auto b = field_with_no_accesses();
  auto c = field_with_no_accesses();
  auto d = field_with_no_accesses();
  auto a = field_with_accesses("b", fcp, std::move(b), "c", fcp, std::move(c),
                               "d", fcp, std::move(d));
  auto root = field_with_accesses("a", fcp, std::move(a));
  const auto results = ResultTree(ast, std::move(root));
}

struct MockFieldGenerator {
  MOCK_METHOD(FieldPtr, get, (gsl::index index), (const));
};

template <ranges::category Cat>
void test_minimal_system_calls_with_element_access(gsl::index index,
                                                   gsl::index size) {
  SCOPED_TRACE(testing::Message()
               << "test_minimal_system_calls_with_element_access<" << Cat
               << ">(" << index << ", " << size << ")");

  const auto ast = generate_ast(fmt::format("a[{}]", index));

  auto a0 = field_with_no_accesses();
  auto mfg = testing::StrictMock<MockFieldGenerator>{};
  const auto normalized_index = (index >= 0) ? index : (size + index);
  EXPECT_CALL(mfg, get(normalized_index))
      .WillOnce(Return(ByMove(std::move(a0))));

  auto arange =
      FieldRange<Cat>{rv::iota(gsl::index{0}, size) |
                      rv::transform([&](auto i) { return mfg.get(i); })};
  const auto fcp = FieldCallParams{};
  auto root = field_with_accesses("a", fcp, std::move(arange));
  const auto results = ResultTree(ast, std::move(root));
}

TEST_F(ResultTreeTest, TestMinimalSystemCallsWithElementAccess) {
  test_minimal_system_calls_with_element_access<input>(3, 5);

  // Only sized input ranges or forward ranges can handle negative indeces
  // efficiently
  test_minimal_system_calls_with_element_access<input | sized>(-3, 5);
  test_minimal_system_calls_with_element_access<forward>(-3, 5);
}

template <ranges::category Cat>
void test_minimal_system_calls_with_slice(std::optional<gsl::index> start,
                                          std::optional<gsl::index> stop,
                                          std::optional<gsl::index> step,
                                          gsl::index size) {
  SCOPED_TRACE(testing::Message()
               << "test_minimal_system_calls_with_slice<" << Cat << ">("
               << fmt::to_string(start) << ", " << fmt::to_string(stop) << ", "
               << fmt::to_string(step) << ", " << size << ")");

  const auto query = fmt::format("a[{}:{}:{}]", start, stop, step);
  const auto ast = generate_ast(query);

  auto mfg = testing::StrictMock<MockFieldGenerator>{};
  auto step_v = step.value_or(1);
  auto start_v = start.value_or(0);
  auto stop_v = stop.value_or(size);
  auto compare = [=](auto i, auto e) {
    return (step_v > 0) ? (i < e) : (i > e);
  };
  if (step_v < 0) {
    start_v = start.value_or(size - 1);
    stop_v = stop.value_or(-1);
  }
  if (start_v < 0) {
    start_v += size;
  }
  if (stop && stop_v < 0) {
    stop_v += size;
  }

  for (auto i = start_v; compare(i, stop_v); i += step_v) {
    EXPECT_CALL(mfg, get(i)).WillOnce(Return(ByMove(field_with_no_accesses())));
  }
  auto arange = FieldRange<Cat>{
      rv::iota(gsl::index{0}, size) |
      rv::transform([&](auto index) { return mfg.get(index); })};
  const auto fcp = FieldCallParams{};
  auto root = field_with_accesses("a", fcp, std::move(arange));
  const auto results = ResultTree(ast, std::move(root));
}

TEST_F(ResultTreeTest, TestMinimalSystemCallsWithSlice) {
  test_minimal_system_calls_with_slice<input>(2, 5, 2, 6);

  // Non-sized input ranges can't handle negative indeces efficiently
  test_minimal_system_calls_with_slice<input | sized>(2, -1, 2, 6);
  test_minimal_system_calls_with_slice<input | sized>(-4, 5, 2, 6);
  test_minimal_system_calls_with_slice<input | sized>(-4, -1, 2, 6);
  test_minimal_system_calls_with_slice<forward>(2, -1, 2, 6);
  test_minimal_system_calls_with_slice<forward>(-4, 5, 2, 6);
  test_minimal_system_calls_with_slice<forward>(-4, -1, 2, 6);

  // Only bidirectional ranges can handle negative steps efficiently
  test_minimal_system_calls_with_slice<bidirectional>(5, 2, -2, 6);
  test_minimal_system_calls_with_slice<bidirectional>(5, -4, -2, 6);
  test_minimal_system_calls_with_slice<bidirectional>(-1, 2, -2, 6);
  test_minimal_system_calls_with_slice<bidirectional>(-1, -4, -2, 6);
}

template <ranges::category Cat>
void test_minimal_system_calls_with_comparison_filter(
    std::string_view member, parser::ComparisonOperator op, gsl::index index,
    gsl::index size) {
  SCOPED_TRACE(testing::Message()
               << "test_minimal_system_calls_with_comparison_filter<" << Cat
               << ">(" << op << ", " << index << ", " << size << ")");

  const auto ast = generate_ast(fmt::format("a[{}{}{}]", member, op, index));

  auto mfg = testing::StrictMock<MockFieldGenerator>{};

  for (int i = 0; i < size; ++i) {
    EXPECT_CALL(mfg, get(i)).WillOnce([&](gsl::index i) -> FieldPtr {
      if (member.empty()) {
        return fake_field(i);
      }
      return field_with_accesses(member, FieldCallParams{}, fake_field());
    });
  }

  auto arange =
      FieldRange<Cat>{rv::iota(gsl::index{0}, size) |
                      rv::transform([&](auto i) { return mfg.get(i); })};
  const auto fcp = FieldCallParams{};
  auto root = field_with_accesses("a", fcp, std::move(arange));
  const auto results = ResultTree(ast, std::move(root));
}

TEST_F(ResultTreeTest, TestMinimalSystemCallsWithComparisonFilter) {
  static constexpr auto size = gsl::index{5};
  for (const auto op : all_comparison_ops) {
    for (const auto i : {0, 3, 5}) {
      for (const auto member : {"", "m"}) {
        test_minimal_system_calls_with_comparison_filter<input>(member, op, i,
                                                                size);
        test_minimal_system_calls_with_comparison_filter<forward>(member, op, i,
                                                                  size);
        test_minimal_system_calls_with_comparison_filter<bidirectional>(
            member, op, i, size);
        test_minimal_system_calls_with_comparison_filter<random_access>(
            member, op, i, size);
        test_minimal_system_calls_with_comparison_filter<input | sized>(
            member, op, i, size);
        test_minimal_system_calls_with_comparison_filter<forward | sized>(
            member, op, i, size);
        test_minimal_system_calls_with_comparison_filter<bidirectional | sized>(
            member, op, i, size);
        test_minimal_system_calls_with_comparison_filter<random_access | sized>(
            member, op, i, size);
      }
    }
  }
}

// -----------------------------------------------------------------------------
// Tree generation tests - to check result trees contain the right structure
// and data.
// -----------------------------------------------------------------------------
TEST_F(ResultTreeTest, TestGeneratedTreeWithSingleCallPerObject) {
  const auto ast = generate_ast("a.b.c");
  auto root = fake_field();
  const auto results = ResultTree{ast, std::move(root)};

  auto c = primitive_tree(0);
  auto b = obj_data_tree("c", std::move(c));
  auto a = obj_data_tree("b", std::move(b));
  const auto expected = obj_data_tree("a", std::move(a));
  EXPECT_EQ(results, expected);
}

TEST_F(ResultTreeTest, TestGeneratedTreeWithMultipleCallsPerObject) {
  const auto ast = generate_ast("a {b c d}");
  auto root = fake_field();
  const auto results = ResultTree{ast, std::move(root)};

  auto b = primitive_tree(0);
  auto c = primitive_tree(0);
  auto d = primitive_tree(0);
  auto a =
      obj_data_tree("b", std::move(b), "c", std::move(c), "d", std::move(d));
  const auto expected = obj_data_tree("a", std::move(a));
  EXPECT_EQ(results, expected);
}

TEST_F(ResultTreeTest, TestGeneratedTreeWithPullup) {
  const auto ast = generate_ast("a.<b");
  const auto results = ResultTree{ast, fake_field()};
  EXPECT_EQ(results, obj_data_tree("a", primitive_tree(0)));
}

TEST_F(ResultTreeTest, TestGeneratedTreeWithDoublePullup) {
  const auto ast = generate_ast("a.<b.<c");
  const auto results = ResultTree{ast, fake_field()};
  EXPECT_EQ(results, obj_data_tree("a", primitive_tree(0)));
}

TEST_F(ResultTreeTest, TestGeneratedTreeWithRootPullup) {
  const auto ast = generate_ast("<a");
  const auto results = ResultTree{ast, fake_field()};
  EXPECT_EQ(results, primitive_tree(0));
}

TEST_F(ResultTreeTest, TestGeneratedTreeWithArrayPullup) {
  const auto ast = generate_ast("a.<b");
  auto root = fake_field(fake_field(fake_field_range(0, 10)));
  const auto results = ResultTree{ast, std::move(root)};

  const auto expected = obj_data_tree("a", int_array_data_tree(0, 10));
  EXPECT_EQ(results, expected);
}

TEST_F(ResultTreeTest, TestGeneratedTreeWithDoubleArrayPullup) {
  const auto ast = generate_ast("a.<b.<c");
  auto a = fake_field([](auto, auto) {
    return to_field_range(input, rv::iota(0, 10) | rv::transform([](int i) {
                                   return fake_field(fake_field_range(0, i));
                                 }));
  });
  const auto results = ResultTree{ast, fake_field(std::move(a))};

  auto expected = obj_data_tree(
      "a", to_array_data_tree(rv::iota(0, 10) | rv::transform([](auto i) {
                                return int_array_data_tree(0, i);
                              })));
  EXPECT_EQ(results, expected);
}

// -----------------------------------------------------------------------------
// Param passing tests
// -----------------------------------------------------------------------------

struct ParamPassingTestCase {
  const char *query_;
  FieldCallParams fcp_;
};

std::ostream &operator<<(std::ostream &os, const ParamPassingTestCase &pptc) {
  os << "(query=" << pptc.query_ << ",fcp=" << pptc.fcp_ << ")";
  return os;
}

struct ResultTreeParamTest
    : MockFieldTest,
      ::testing::WithParamInterface<ParamPassingTestCase> {};

TEST_P(ResultTreeParamTest, TestParamPassing) {
  const auto [query, params] = GetParam();
  const auto ast = generate_ast(query);
  auto a = field_with_no_accesses();
  auto root = field_with_accesses("a", params, std::move(a));
  const auto results = ResultTree(ast, std::move(root));
}

INSTANTIATE_TEST_SUITE_P(
    ResultTreeParamTestInstantiation, ResultTreeParamTest,
    testing::Values(
        ParamPassingTestCase{"a(\"str\")", params("str")},
        ParamPassingTestCase{"a(\"\")", params("")},
        ParamPassingTestCase{"a(0)", params(0)},
        ParamPassingTestCase{"a(1)", params(1)},
        ParamPassingTestCase{"a(-1)", params(-1)},
        ParamPassingTestCase{"a(true)", params(true)},
        ParamPassingTestCase{"a(false)", params(false)},
        ParamPassingTestCase{"a(n=\"str\")", params(named("n", "str"))},
        ParamPassingTestCase{"a(n=\"\")", params(named("n", ""))},
        ParamPassingTestCase{"a(n=0)", params(named("n", 0))},
        ParamPassingTestCase{"a(n=1)", params(named("n", 1))},
        ParamPassingTestCase{"a(n=-1)", params(named("n", -1))},
        ParamPassingTestCase{"a(n=true)", params(named("n", true))},
        ParamPassingTestCase{"a(n=false)", params(named("n", false))},
        ParamPassingTestCase{"a(\"str\", 1, true, n1=\"\", n2=-1, n3=false)",
                             params("str", 1, true, named("n1", ""),
                                    named("n2", -1), named("n3", false))}));

// -----------------------------------------------------------------------------
// Filter tests
// -----------------------------------------------------------------------------

void test_element_access(ranges::category cat, gsl::index index,
                         gsl::index size) {
  SCOPED_TRACE(testing::Message() << "test_element_access("
                                  << "cat=" << cat << ", "
                                  << "index=" << index << ", "
                                  << "size=" << size << ")");

  const auto ast = generate_ast(fmt::format("a[{}]", index));

  const auto normalized_index = (index >= 0) ? index : (size + index);
  auto arange = to_field_range(
      cat, rv::iota(PrimitiveInt{0}, size) |
               rv::transform([](auto i) { return fake_field(i); }));
  auto root = fake_field(std::move(arange));
  const auto results = ResultTree(ast, std::move(root));
  ASSERT_TRUE(std::holds_alternative<ObjData>(results.data()));
  const auto &res_root = std::get<ObjData>(results.data());
  EXPECT_EQ(res_root.size(), 1);
  EXPECT_EQ(res_root[0].first, "a");
  ASSERT_TRUE(std::holds_alternative<Primitive>(res_root[0].second.data()));
  const auto &res_a_prim = std::get<Primitive>(res_root[0].second.data());
  ASSERT_TRUE(std::holds_alternative<PrimitiveInt>(res_a_prim));
  const auto &res_a = std::get<PrimitiveInt>(res_a_prim);
  EXPECT_EQ(res_a, normalized_index);
}

TEST_F(ResultTreeTest, TestElementAccess) {
  static constexpr auto size = gsl::index{4};

  for (const auto cat : all_categories) {
    for (const auto index : rv::iota(-size, size)) {
      test_element_access(cat, index, size);
    }
  }
}

TEST_F(ResultTreeTest, TestElementAccessOutOfRange) {
  for (const auto cat : all_categories) {
    EXPECT_THROW({ test_element_access(cat, 10, 10); }, OutOfRangeError);
    EXPECT_THROW({ test_element_access(cat, 11, 10); }, OutOfRangeError);
    EXPECT_THROW({ test_element_access(cat, -11, 10); }, OutOfRangeError);
    EXPECT_THROW({ test_element_access(cat, -12, 10); }, OutOfRangeError);
  }
}

void test_slice(ranges::category cat, std::optional<gsl::index> start,
                std::optional<gsl::index> stop, std::optional<gsl::index> step,
                gsl::index size) {
  SCOPED_TRACE(testing::Message() << "test_slice("
                                  << "cat=" << cat << ", "
                                  << "start=" << fmt::to_string(start) << ", "
                                  << "stop=" << fmt::to_string(stop) << ", "
                                  << "step=" << fmt::to_string(step) << ", "
                                  << "size=" << size << ")");

  const auto query = fmt::format("a[{}:{}:{}]", start, stop, step);
  const auto ast = generate_ast(query);
  auto step_v = step.value_or(1);
  auto start_v = gsl::index{0};
  auto stop_v = gsl::index{0};
  auto compare = [=](auto i, auto e) {
    return (step_v > 0) ? (i < e && i < size) : (i > e && i > -1);
  };
  if (step_v < 0) {
    start_v = start.value_or(size - 1);
    if (start_v < 0) {
      start_v += size;
    }
    if (start_v >= size) {
      start_v = size - 1;
    }

    stop_v = stop.value_or(-1);
    if (stop && stop_v < 0) {
      stop_v += size;
    }
    if (stop_v < -1) {
      stop_v = -1;
    }
  } else {
    start_v = start.value_or(0);
    if (start_v < 0) {
      start_v += size;
    }
    if (start_v < 0) {
      start_v = 0;
    }

    stop_v = stop.value_or(size);
    if (stop && stop_v < 0) {
      stop_v += size;
    }
    if (stop_v > size) {
      stop_v = size;
    }
  }

  auto expected_a_data = ArrayData{};
  for (auto i = start_v; compare(i, stop_v); i += step_v) {
    expected_a_data.emplace_back(PrimitiveInt{i});
  }
  auto expected_a = ResultTree{std::move(expected_a_data)};
  auto expected_root = obj_data_tree("a", std::move(expected_a));

  auto system_a = to_field_range(
      cat, rv::iota(PrimitiveInt{0}, size) |
               rv::transform([](auto i) { return fake_field(i); }));

  auto system_root = fake_field(std::move(system_a));
  const auto root = ResultTree(ast, std::move(system_root));
  ASSERT_EQ(root, expected_root);
}

TEST_F(ResultTreeTest, TestSlice) {
  using OIL = std::initializer_list<std::optional<gsl::index>>;
  auto indeces = OIL{std::nullopt, 0, 1, 2, 3, 4, 5, 6, -1, -2, -3, -4, -5, -6};
  auto steps = OIL{std::nullopt, -3, -2, -1, 1, 2, 3};

  auto arg_packs =
      rv::cartesian_product(all_categories, indeces, indeces, steps);

  for (auto arg_pack : arg_packs) {
    test_slice(std::get<0>(arg_pack), std::get<1>(arg_pack),
               std::get<2>(arg_pack), std::get<3>(arg_pack), 5);
  }
}

void test_comparison_filter(std::string_view member,
                            parser::ComparisonOperator op, ranges::category cat,
                            gsl::index index, gsl::index size) {
  SCOPED_TRACE(testing::Message() << "test_comparison_filter("
                                  << "member=" << member << ", "
                                  << "op=" << op << ", "
                                  << "cat=" << cat << ", "
                                  << "index=" << index << ", "
                                  << "size=" << size << ")");
  const auto ast = generate_ast(fmt::format("a[{}{}{}]", member, op, index));

  auto transformer = std::function<FieldPtr(PrimitiveInt)>{};
  if (member.empty()) {
    transformer = [](auto i) { return fake_field(i); };
  } else {
    transformer = [](auto i) {
      return fake_field(
          [=](SQ_MU std::string_view member,
              SQ_MU const FieldCallParams &params) { return fake_field(i); });
    };
  }

  auto arange = to_field_range(cat, rv::iota(PrimitiveInt{0}, size) |
                                        rv::transform(transformer));

  auto root = fake_field(std::move(arange));
  const auto results = ResultTree(ast, std::move(root));
  ASSERT_TRUE(std::holds_alternative<ObjData>(results.data()));
  const auto &res_root = std::get<ObjData>(results.data());
  EXPECT_EQ(res_root.size(), 1);
  EXPECT_EQ(res_root[0].first, "a");
  ASSERT_TRUE(std::holds_alternative<ArrayData>(res_root[0].second.data()));
  const auto &res_a_arr = std::get<ArrayData>(res_root[0].second.data());

  // Get the half-open range of indeces for which we expect the comparison to
  // be true
  const auto [begin_matching,
              end_matching] = [&]() -> std::pair<gsl::index, gsl::index> {
    switch (op) {
    case parser::ComparisonOperator::GreaterThanOrEqualTo:
      return std::pair{index, size};
    case parser::ComparisonOperator::GreaterThan:
      return std::pair{index + 1, size};
    case parser::ComparisonOperator::LessThanOrEqualTo:
      return std::pair{0, index + 1};
    case parser::ComparisonOperator::LessThan:
      return std::pair{0, index};
    case parser::ComparisonOperator::Equals:
      return std::pair{index, index + 1};
    }
    ASSERT(false);
    return std::pair{0, 0};
  }();

  const auto noof_matches = end_matching - begin_matching;
  ASSERT_EQ(ranges::size(res_a_arr), noof_matches);

  for (auto i = 0; i < noof_matches; ++i) {
    const auto &res_a_element = res_a_arr.at(to_size(i)).data();
    ASSERT_TRUE(std::holds_alternative<Primitive>(res_a_element));
    const auto &prim = std::get<Primitive>(res_a_element);
    ASSERT_TRUE(std::holds_alternative<PrimitiveInt>(prim));
    const auto &primint = std::get<PrimitiveInt>(prim);
    if (member.empty()) {
      EXPECT_EQ(primint, begin_matching + i);
    } else {
      EXPECT_EQ(primint, 0);
    }
  }
}

TEST_F(ResultTreeTest, TestComparisonFilter) {
  static constexpr auto size = gsl::index{4};

  for (const auto cat : all_categories) {
    for (const auto op : all_comparison_ops) {
      for (const auto index : rv::iota(0, size)) {
        for (const auto member : {"", "m"}) {
          test_comparison_filter(member, op, cat, index, size);
        }
      }
    }
  }
}

TEST_F(ResultTreeTest, TestNotAnArrayError) {
  for (const auto &query : {"a[0]", "a[::]"}) {
    SCOPED_TRACE(testing::Message() << "query=" << query);

    const auto ast = generate_ast(query);
    auto root = fake_field();

    EXPECT_THROW({ auto results = ResultTree(ast, std::move(root)); },
                 NotAnArrayError);
  }
}

} // namespace
} // namespace sq::test
