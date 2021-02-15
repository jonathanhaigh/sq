#include "results/results.h"

#include "ast/ast.h"
#include "results_test_util.h"
#include "test/FieldCallParams_test_util.h"
#include "util/strutil.h"

#include <gtest/gtest.h>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <utility>
#include <sstream>

namespace sq::test {

using namespace sq::results;

// -----------------------------------------------------------------------------
// Minimal system call tests:
// Ensure that the results gathering library doesn't call into the system more
// than it needs to. The results gathering code should assume that all calls
// into the system are expensive and should cache results if repeated access is
// required.
// -----------------------------------------------------------------------------
TEST(ResultTreeTest, TestMinimalSystemCallsWithSingleCallPerObject)
{
    const auto ast = ast::generate_ast("a.b.c");
    const auto fcp = FieldCallParams{};
    auto c = field_with_one_primitive_access(PrimitiveInt{1});
    auto b = field_with_accesses("c", fcp, std::move(c));
    auto a = field_with_accesses("b", fcp, std::move(b));
    auto root = field_with_accesses("a", fcp, std::move(a));
    const auto results = ResultTree(ast, std::move(root));
}

TEST(ResultTreeTest, TestMinimalSystemCallsWithMultipleCallsPerObject)
{
    const auto ast = ast::generate_ast("a { b c d }");
    const auto fcp = FieldCallParams{};
    auto b = field_with_one_primitive_access(PrimitiveInt{1});
    auto c = field_with_one_primitive_access(PrimitiveInt{2});
    auto d = field_with_one_primitive_access(PrimitiveInt{3});
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
    MOCK_METHOD(FieldPtr, get, (std::ptrdiff_t index), (const));
};

template <typename T>
struct TestWithFieldRangeType : testing::Test { };

using FieldRangeTypes = testing::Types<
    FieldRange<input>,
    FieldRange<forward>,
    FieldRange<bidirectional>,
    FieldRange<random_access>,
    FieldRange<input | sized>,
    FieldRange<forward | sized>,
    FieldRange<bidirectional | sized>,
    FieldRange<random_access | sized>
>;
TYPED_TEST_SUITE(TestWithFieldRangeType, FieldRangeTypes,);

template <ranges::category Cat>
void test_minimal_system_calls_with_element_access(
    std::ptrdiff_t index,
    std::ptrdiff_t size
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
    const auto ast = ast::generate_ast(ss.str());

    auto a0 = field_with_one_primitive_access(PrimitiveInt{1});
    auto mfg = testing::StrictMock<MockFieldGenerator>{};
    const auto normalized_index = (index >= 0)? index : (size + index);
    EXPECT_CALL(mfg, get(normalized_index))
        .WillOnce(Return(ByMove(std::move(a0))));

    auto arange = FieldRange<Cat>{
        ranges::views::iota(std::ptrdiff_t{0}, size) |
        ranges::views::transform(
            [&](int i) { return mfg.get(i); }
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
    std::optional<std::ptrdiff_t> start,
    std::optional<std::ptrdiff_t> stop,
    std::optional<std::ptrdiff_t> step,
    std::ptrdiff_t size
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
    const auto ast = ast::generate_ast(ss.str());
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
                field_with_one_primitive_access(PrimitiveInt{1})
            )));
    }
    auto arange = FieldRange<Cat>{
        ranges::views::iota(std::ptrdiff_t{0}, size) |
        ranges::views::transform(
            [&](int index) { return mfg.get(index); }
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
    const auto ast = ast::generate_ast("a.b.c");
    auto root = ResultView{std::make_unique<FakeField>()};
    const auto results = ResultTree{ast, std::move(root)};

    auto c = ResultTree{PrimitiveInt{0}};
    auto b = obj_data_tree("c", std::move(c));
    auto a = obj_data_tree("b", std::move(b));
    const auto expected = obj_data_tree("a", std::move(a));
    EXPECT_EQ(results, expected);
}

TEST(ResultTreeTest, TestGeneratedTreeWithMultipleCallsPerObject)
{
    const auto ast = ast::generate_ast("a {b c d}");
    auto root = ResultView{std::make_unique<FakeField>()};
    const auto results = ResultTree{ast, std::move(root)};

    auto b = ResultTree{PrimitiveInt{0}};
    auto c = ResultTree{PrimitiveInt{0}};
    auto d = ResultTree{PrimitiveInt{0}};
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

using ParamPassingTestCase = std::tuple<const char*, FieldCallParams>;
class ResultTreeParamTest
    : public testing::TestWithParam<ParamPassingTestCase>
{ };

TEST_P(ResultTreeParamTest, TestParamPassing)
{
    const auto [ query, params ] = GetParam();
    const auto ast = ast::generate_ast(query);
    auto a = field_with_one_primitive_access(PrimitiveInt{0});
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

} // namespace sq::test
