/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/Serializer.h"
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
#include <rapidjson/document.h>
#include <utility>

namespace sq::test {
namespace {

using namespace sq::results;
namespace rj = rapidjson;

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

std::string generate_results(const parser::Ast &ast, const FieldPtr &root) {
  std::ostringstream os{};
  auto serializer = get_serializer(os);
  generate_results(ast, root, *serializer);
  return os.str();
}

struct ResultsTest : public MockFieldTest {};

// -----------------------------------------------------------------------------
// Minimal system call tests:
// Ensure that the results gathering library doesn't call into the system more
// than it needs to. The results gathering code should assume that all calls
// into the system are expensive and should cache results if repeated access is
// required.
// -----------------------------------------------------------------------------
TEST_F(ResultsTest, TestMinimalSystemCallsWithSingleCallPerObject) {
  const auto ast = generate_ast("a.b.c");
  const auto fcp = FieldCallParams{};
  auto c = field_with_no_accesses();
  auto b = field_with_accesses("c", fcp, std::move(c));
  auto a = field_with_accesses("b", fcp, std::move(b));
  auto root = field_with_accesses("a", fcp, std::move(a));
  generate_results(ast, std::move(root));
}

TEST_F(ResultsTest, TestMinimalSystemCallsWithMultipleCallsPerObject) {
  const auto ast = generate_ast("a { b c d }");
  const auto fcp = FieldCallParams{};
  auto b = field_with_no_accesses();
  auto c = field_with_no_accesses();
  auto d = field_with_no_accesses();
  auto a = field_with_accesses("b", fcp, std::move(b), "c", fcp, std::move(c),
                               "d", fcp, std::move(d));
  auto root = field_with_accesses("a", fcp, std::move(a));
  generate_results(ast, std::move(root));
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
  generate_results(ast, std::move(root));
}

TEST_F(ResultsTest, TestMinimalSystemCallsWithElementAccess) {
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
  generate_results(ast, std::move(root));
}

TEST_F(ResultsTest, TestMinimalSystemCallsWithSlice) {
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
  generate_results(ast, std::move(root));
}

TEST_F(ResultsTest, TestMinimalSystemCallsWithComparisonFilter) {
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
// Tree generation tests - to check result trees contain the right
// structure and data.
// -----------------------------------------------------------------------------

struct SimpleResultsTestCase {

  SimpleResultsTestCase(const char *query, const char *expected_json,
                        FieldPtr system_root)
      : query{query}, expected_json{expected_json}, system_root{system_root} {}

  SimpleResultsTestCase(const char *query, const char *expected_json)
      : SimpleResultsTestCase{query, expected_json, fake_field()} {}

  const char *query;
  const char *expected_json;
  FieldPtr system_root;
};

std::ostream &operator<<(std::ostream &os, const SimpleResultsTestCase &tc) {
  os << tc.query << "; " << tc.expected_json;
  return os;
}
struct SimpleResultsTest : ::testing::TestWithParam<SimpleResultsTestCase> {};

TEST_P(SimpleResultsTest, TestSimpleTreeGeneration) {
  auto [query, expected, root] = GetParam();
  const auto ast = generate_ast(query);
  auto results = generate_results(ast, std::move(root));
  expect_equivalent_json(results, expected);
}

INSTANTIATE_TEST_SUITE_P(
    SimpleQueries, SimpleResultsTest,
    ::testing::Values(
        SimpleResultsTestCase{"a.b.c", R"({ "a": { "b": { "c": 0 } } })"},
        SimpleResultsTestCase{"a {b c d}",
                              R"({ "a": { "b": 0, "c": 0, "d": 0 } })"},
        SimpleResultsTestCase{"a.<b", R"({ "a": 0 })"},
        SimpleResultsTestCase{"a.<b.<c", R"({ "a": 0 })"},
        SimpleResultsTestCase{"<a", "0"}, SimpleResultsTestCase{"<a.<b", "0"},
        SimpleResultsTestCase{"<b", "[0, 1, 2, 3, 4, 5]",
                              fake_field(fake_field_range(0, 5))},

        SimpleResultsTestCase{"<b.<c", "[[], [0], [0, 1, 2], [0, 1, 2, 3]]",
                              fake_field([](auto, auto) {
                                return to_field_range(
                                    input,
                                    rv::iota(0, 5) | rv::transform([](int i) {
                                      return fake_field(fake_field_range(0, i));
                                    }));
                              })}));

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

struct ResultsParamTest : MockFieldTest,
                          ::testing::WithParamInterface<ParamPassingTestCase> {
};

TEST_P(ResultsParamTest, TestParamPassing) {
  const auto [query, params] = GetParam();
  const auto ast = generate_ast(query);
  auto a = field_with_no_accesses();
  auto root = field_with_accesses("a", params, std::move(a));
  generate_results(ast, std::move(root));
}

INSTANTIATE_TEST_SUITE_P(
    ResultsParamTestInstantiation, ResultsParamTest,
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

  const auto ast = generate_ast(fmt::format("<a[{}]", index));

  const auto normalized_index = (index >= 0) ? index : (size + index);
  auto arange = to_field_range(
      cat, rv::iota(PrimitiveInt{0}, size) |
               rv::transform([](auto i) { return fake_field(i); }));
  auto root = fake_field(std::move(arange));
  const auto results = generate_results(ast, std::move(root));

  expect_equivalent_json(results, fmt::to_string(normalized_index));
}

TEST_F(ResultsTest, TestElementAccess) {
  static constexpr auto size = gsl::index{4};

  for (const auto cat : all_categories) {
    for (const auto index : rv::iota(-size, size)) {
      test_element_access(cat, index, size);
    }
  }
}

TEST_F(ResultsTest, TestElementAccessOutOfRange) {
  for (const auto cat : all_categories) {
    EXPECT_THROW({ test_element_access(cat, 10, 10); }, OutOfRangeError);
    EXPECT_THROW({ test_element_access(cat, 11, 10); }, OutOfRangeError);
    EXPECT_THROW({ test_element_access(cat, -11, 10); }, OutOfRangeError);
    EXPECT_THROW({ test_element_access(cat, -12, 10); }, OutOfRangeError);
  }
}

std::vector<gsl::index> get_slice_indeces(std::optional<gsl::index> start,
                                          std::optional<gsl::index> stop,
                                          std::optional<gsl::index> step,
                                          gsl::index size) {
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

  std::vector<gsl::index> ret{};
  for (auto i = start_v; compare(i, stop_v); i += step_v) {
    ret.push_back(i);
  }
  return ret;
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

  const auto query = fmt::format("<a[{}:{}:{}]", start, stop, step);
  const auto ast = generate_ast(query);
  auto root = fake_field(to_field_range(
      cat, rv::iota(PrimitiveInt{0}, size) |
               rv::transform([](auto i) { return fake_field(i); })));

  const auto results = generate_results(ast, std::move(root));

  auto expected_indeces = get_slice_indeces(start, stop, step, size);
  auto expected_json =
      fmt::format("[{}]", fmt::join(std::move(expected_indeces), ", "));

  expect_equivalent_json(results, expected_json);
}

TEST_F(ResultsTest, TestSlice) {
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
  const auto ast = generate_ast(fmt::format("<a[{}{}{}]", member, op, index));

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

  auto root = to_field_range(cat, rv::iota(PrimitiveInt{0}, size) |
                                      rv::transform(transformer));

  const auto results = generate_results(ast, fake_field(std::move(root)));

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

  const auto num_matches = end_matching - begin_matching;
  auto expected_values =
      member.empty() ? ranges::views::iota(begin_matching, end_matching) |
                           ranges::to<std::vector>()
                     : std::vector<gsl::index>(to_size(num_matches), 0);

  auto expected_json = fmt::format("[{}]", fmt::join(expected_values, ", "));
  expect_equivalent_json(results, expected_json);
}

TEST_F(ResultsTest, TestComparisonFilter) {
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

TEST_F(ResultsTest, TestNotAnArrayError) {
  for (const auto &query : {"a[0]", "a[::]"}) {
    SCOPED_TRACE(testing::Message() << "query=" << query);

    const auto ast = generate_ast(query);
    auto root = fake_field();

    EXPECT_THROW({ generate_results(ast, std::move(root)); }, NotAnArrayError);
  }
}

} // namespace
} // namespace sq::test
