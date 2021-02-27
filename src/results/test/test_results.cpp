/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/results.h"

#include "parser/Ast.h"
#include "parser/Parser.h"
#include "parser/TokenView.h"
#include "common_types/NotAnArrayError.h"
#include "common_types/OutOfRangeError.h"
#include "test/FieldCallParams_test_util.h"
#include "test/results_test_util.h"
#include "util/strutil.h"

#include <gsl/gsl>
#include <gtest/gtest.h>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <utility>
#include <sstream>

namespace sq::test {
namespace {

using namespace sq::results;

parser::Ast generate_ast(std::string_view query)
{
    auto tokens = parser::TokenView{query};
    auto parser = parser::Parser{tokens};
    return parser.parse();
}

// -----------------------------------------------------------------------------
// Minimal system call tests:
// Ensure that the results gathering library doesn't call into the system more
// than it needs to. The results gathering code should assume that all calls
// into the system are expensive and should cache results if repeated access is
// required.
// -----------------------------------------------------------------------------
TEST(ResultTreeTest, TestMinimalSystemCallsWithSingleCallPerObject)
{
    const auto ast = generate_ast("a.b.c");
    const auto fcp = FieldCallParams{};
    auto c = field_with_one_primitive_access(1);
    auto b = field_with_accesses("c", fcp, std::move(c));
    auto a = field_with_accesses("b", fcp, std::move(b));
    auto root = field_with_accesses("a", fcp, std::move(a));
    const auto results = ResultTree(ast, std::move(root));
}

TEST(ResultTreeTest, TestMinimalSystemCallsWithMultipleCallsPerObject)
{
    const auto ast = generate_ast("a { b c d }");
    const auto fcp = FieldCallParams{};
    auto b = field_with_one_primitive_access(1);
    auto c = field_with_one_primitive_access(2);
    auto d = field_with_one_primitive_access(3);
    auto a = field_with_accesses(
        "b", fcp, std::move(b),
        "c", fcp, std::move(c),
        "d", fcp, std::move(d)
    );
    auto root = field_with_accesses("a", fcp, std::move(a));
    const auto results = ResultTree(ast, std::move(root));
}

struct MockFieldGenerator
{
    MOCK_METHOD(FieldPtr, get, (gsl::index index), (const));
};

template <ranges::category Cat>
void test_minimal_system_calls_with_element_access(
    gsl::index index,
    gsl::index size
)
{
    SCOPED_TRACE(testing::Message()
        << "test_minimal_system_calls_with_element_access<"
        << Cat << ">("
        << index << ", "
        << size << ")"
    );

    auto ss = std::stringstream{};
    ss << "a[" << index << "]";
    const auto ast = generate_ast(ss.str());

    auto a0 = field_with_one_primitive_access(1);
    auto mfg = testing::StrictMock<MockFieldGenerator>{};
    const auto normalized_index = (index >= 0)? index : (size + index);
    EXPECT_CALL(mfg, get(normalized_index))
        .WillOnce(Return(ByMove(std::move(a0))));

    auto arange = FieldRange<Cat>{
        ranges::views::iota(gsl::index{0}, size) |
        ranges::views::transform(
            [&](auto i) { return mfg.get(i); }
        )
    };
    const auto fcp = FieldCallParams{};
    auto root = field_with_accesses("a", fcp, std::move(arange));
    const auto results = ResultTree(ast, std::move(root));
}

TEST(ResultTreeTest, TestMinimalSystemCallsWithElementAccess)
{
    test_minimal_system_calls_with_element_access<input>(10, 20);

    // Only sized input ranges or forward ranges can handle negative indeces
    // efficiently
    test_minimal_system_calls_with_element_access<input | sized>(-10, 20);
    test_minimal_system_calls_with_element_access<forward>(-10, 20);
}

template <ranges::category Cat>
void test_minimal_system_calls_with_slice(
    std::optional<gsl::index> start,
    std::optional<gsl::index> stop,
    std::optional<gsl::index> step,
    gsl::index size
)
{
    SCOPED_TRACE(testing::Message()
        << "test_minimal_system_calls_with_slice<"
        << Cat << ">("
        << util::optional_to_str(start) << ", "
        << util::optional_to_str(stop) << ", "
        << util::optional_to_str(step) << ", "
        << size << ")"
    );

    auto ss = std::ostringstream{};
    ss << "a["
       << util::optional_to_str(start) << ":"
       << util::optional_to_str(stop) << ":"
       << util::optional_to_str(step) << "]";
    const auto ast = generate_ast(ss.str());
    auto mfg = testing::StrictMock<MockFieldGenerator>{};
    auto step_v = step.value_or(1);
    auto start_v = start.value_or(0);
    auto stop_v = stop.value_or(size);
    auto compare = [=](auto i, auto e) {
        return (step_v > 0)? (i < e) : (i > e);
    };
    if (step_v < 0)
    {
        start_v = start.value_or(size - 1);
        stop_v = stop.value_or(-1);
    }
    if (start_v < 0) { start_v += size; }
    if (stop && stop_v < 0) { stop_v += size; }

    for (auto i = start_v; compare(i, stop_v); i += step_v)
    {
        EXPECT_CALL(mfg, get(i))
            .WillOnce(Return(ByMove(
                field_with_one_primitive_access(1)
            )));
    }
    auto arange = FieldRange<Cat>{
        ranges::views::iota(gsl::index{0}, size) |
        ranges::views::transform(
            [&](auto index) { return mfg.get(index); }
        )
    };
    const auto fcp = FieldCallParams{};
    auto root = field_with_accesses("a", fcp, std::move(arange));
    const auto results = ResultTree(ast, std::move(root));
}

TEST(ResultTreeTest, TestMinimalSystemCallsWithSlice)
{
    test_minimal_system_calls_with_slice<input>(5, 15, 2, 20);

    // Non-sized input ranges can't handle negative indeces efficiently
    test_minimal_system_calls_with_slice<input | sized>(5, -5, 2, 20);
    test_minimal_system_calls_with_slice<input | sized>(-15, 15, 2, 20);
    test_minimal_system_calls_with_slice<input | sized>(-15, -5, 2, 20);
    test_minimal_system_calls_with_slice<forward>(5, -5, 2, 20);
    test_minimal_system_calls_with_slice<forward>(-15, 15, 2, 20);
    test_minimal_system_calls_with_slice<forward>(-15, -5, 2, 20);

    // Only bidirectional ranges can handle negative steps efficiently
    test_minimal_system_calls_with_slice<bidirectional>(15, 5, -2, 20);
    test_minimal_system_calls_with_slice<bidirectional>(15, -15, -2, 20);
    test_minimal_system_calls_with_slice<bidirectional>(-5, 5, -2, 20);
    test_minimal_system_calls_with_slice<bidirectional>(-5, -15, -2, 20);
}

// -----------------------------------------------------------------------------
// Tree generation tests - to check result trees contain the right structure
// and data.
// -----------------------------------------------------------------------------
TEST(ResultTreeTest, TestGeneratedTreeWithSingleCallPerObject)
{
    const auto ast = generate_ast("a.b.c");
    auto root = ResultView{std::make_unique<FakeField>()};
    const auto results = ResultTree{ast, std::move(root)};

    auto c = primitive_tree(0);
    auto b = obj_data_tree("c", std::move(c));
    auto a = obj_data_tree("b", std::move(b));
    const auto expected = obj_data_tree("a", std::move(a));
    EXPECT_EQ(results, expected);
}

TEST(ResultTreeTest, TestGeneratedTreeWithMultipleCallsPerObject)
{
    const auto ast = generate_ast("a {b c d}");
    auto root = ResultView{std::make_unique<FakeField>()};
    const auto results = ResultTree{ast, std::move(root)};

    auto b = primitive_tree(0);
    auto c = primitive_tree(0);
    auto d = primitive_tree(0);
    auto a = obj_data_tree(
        "b", std::move(b),
        "c", std::move(c),
        "d", std::move(d)
    );
    const auto expected = obj_data_tree("a", std::move(a));
    EXPECT_EQ(results, expected);
}

// -----------------------------------------------------------------------------
// Param passing tests
// -----------------------------------------------------------------------------

using ParamPassingTestCase = std::tuple<std::string_view, FieldCallParams>;
class ResultTreeParamTest
    : public testing::TestWithParam<ParamPassingTestCase>
{ };

TEST_P(ResultTreeParamTest, TestParamPassing)
{
    const auto [ query, params ] = GetParam();
    const auto ast = generate_ast(query);
    auto a = field_with_one_primitive_access(0);
    auto root = field_with_accesses("a", params, std::move(a));
    const auto results = ResultTree(ast, std::move(root));
}

INSTANTIATE_TEST_SUITE_P(
    ResultTreeParamTestInstantiation,
    ResultTreeParamTest,
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
        ParamPassingTestCase{
            "a(\"str\", 1, true, n1=\"\", n2=-1, n3=false)",
            params(
                "str", 1, true,
                named("n1", ""), named("n2", -1), named("n3", false)
            )
        }
    )
);

// -----------------------------------------------------------------------------
// Filter tests
// -----------------------------------------------------------------------------

void test_element_access(
    ranges::category cat,
    gsl::index index,
    gsl::index size
)
{
    SCOPED_TRACE(testing::Message()
        << "test_element_access("
        << "cat=" << cat << ", "
        << "index=" << index << ", "
        << "size=" << size << ")"
    );
    auto ss = std::stringstream{};
    ss << "a[" << index << "]";
    const auto ast = generate_ast(ss.str());

    const auto normalized_index = (index >= 0)? index : (size + index);
    auto arange = to_field_range(cat,
        ranges::views::iota(PrimitiveInt{0}, size) |
        ranges::views::transform(
            [](auto i) {
                return std::make_unique<FakeField>(i);
            }
        )
    );
    auto root = std::make_unique<FakeField>(std::move(arange));
    const auto results = ResultTree(ast, std::move(root));
    ASSERT_TRUE(std::holds_alternative<ObjData>(results.data()));
    const auto& res_root = std::get<ObjData>(results.data());
    EXPECT_EQ(res_root.size(), 1);
    EXPECT_EQ(res_root[0].first, "a");
    ASSERT_TRUE(std::holds_alternative<Primitive>(res_root[0].second.data()));
    const auto& res_a_prim = std::get<Primitive>(res_root[0].second.data());
    ASSERT_TRUE(std::holds_alternative<PrimitiveInt>(res_a_prim));
    const auto& res_a = std::get<PrimitiveInt>(res_a_prim);
    EXPECT_EQ(res_a, normalized_index);
}

TEST(ResultTreeTest, TestElementAccess)
{
    static constexpr auto size = gsl::index{10};

    for (const auto cat : all_categories)
    {
        for (const auto index : ranges::views::iota(-size, size))
        {
            test_element_access(cat, index, size);
        }
    }
}

TEST(ResultTreeTest, TestElementAccessOutOfRange)
{
    for (const auto cat : all_categories)
    {
        EXPECT_THROW({
            test_element_access(cat, 10, 10);
        }, OutOfRangeError);
        EXPECT_THROW({
            test_element_access(cat, 11, 10);
        }, OutOfRangeError);
        EXPECT_THROW({
            test_element_access(cat, -11, 10);
        }, OutOfRangeError);
        EXPECT_THROW({
            test_element_access(cat, -12, 10);
        }, OutOfRangeError);
    }
}

void test_slice(
    ranges::category cat,
    std::optional<gsl::index> start,
    std::optional<gsl::index> stop,
    std::optional<gsl::index> step,
    gsl::index size
)
{
    SCOPED_TRACE(testing::Message()
        << "test_slice("
        << "cat=" << cat << ", "
        << "start=" << util::optional_to_str(start) << ", "
        << "stop=" << util::optional_to_str(stop) << ", "
        << "step=" << util::optional_to_str(step) << ", "
        << "size=" << size << ")"
    );
    auto ss = std::ostringstream{};
    ss << "a["
       << util::optional_to_str(start) << ":"
       << util::optional_to_str(stop) << ":"
       << util::optional_to_str(step) << "]";
    const auto ast = generate_ast(ss.str());
    auto step_v = step.value_or(1);
    auto start_v = gsl::index{0};
    auto stop_v = gsl::index{0};
    auto compare = [=](auto i, auto e) {
        return (step_v > 0)? (i < e && i < size) : (i > e && i > -1);
    };
    if (step_v < 0)
    {
        start_v = start.value_or(size - 1);
        if (start_v < 0) { start_v += size; }
        if (start_v >= size) { start_v = size - 1; }

        stop_v = stop.value_or(-1);
        if (stop && stop_v < 0) { stop_v += size; }
        if (stop_v < -1) { stop_v = -1; }
    }
    else 
    {
        start_v = start.value_or(0);
        if (start_v < 0) { start_v += size; }
        if (start_v < 0) { start_v = 0; }

        stop_v = stop.value_or(size);
        if (stop && stop_v < 0) { stop_v += size; }
        if (stop_v > size) { stop_v = size; }
    }

    auto expected_a_data = ArrayData{};
    for (auto i = start_v; compare(i, stop_v); i += step_v)
    {
        expected_a_data.emplace_back(PrimitiveInt{i});
    }
    auto expected_a = ResultTree{std::move(expected_a_data)};
    auto expected_root = obj_data_tree("a", std::move(expected_a));

    auto system_a = to_field_range(cat,
        ranges::views::iota(PrimitiveInt{0}, size) |
        ranges::views::transform(
            [](auto i) {
                return std::make_unique<FakeField>(i);
            }
        )
    );

    auto system_root = std::make_unique<FakeField>(std::move(system_a));
    const auto root = ResultTree(ast, std::move(system_root));
    ASSERT_EQ(root, expected_root);
}

TEST(ResultTreeTest, TestSlice)
{
    using OIL = std::initializer_list<std::optional<gsl::index>>;
    auto indeces = OIL{
        std::nullopt, 0, 1, 2, 3, 4, 5, 6, -1, -2, -3, -4, -5, -6
    };
    auto steps = OIL{ std::nullopt, -3, -2, -1, 1, 2, 3 };

    auto arg_packs = ranges::views::cartesian_product(
        all_categories, indeces, indeces, steps
    );

    for (auto arg_pack : arg_packs)
    {
        test_slice(
            std::get<0>(arg_pack),
            std::get<1>(arg_pack),
            std::get<2>(arg_pack),
            std::get<3>(arg_pack),
            5
        );
    }
}

TEST(ResultTreeTest, TestNotAnArrayError)
{
    for (const auto& query : { "a[0]", "a[::]" })
    {
        SCOPED_TRACE(testing::Message() << "query=" << query);

        const auto ast = generate_ast(query);
        auto root = std::make_unique<FakeField>();

        EXPECT_THROW({
            auto results = ResultTree(ast, std::move(root));
        }, NotAnArrayError);
    }
}

} // namespace
} // namespace sq::test
