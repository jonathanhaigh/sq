#include "results_test_util.h"

#include "results/results.h"

#include "ast/ast.h"

#include <gtest/gtest.h>

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

} // namespace sq::test
