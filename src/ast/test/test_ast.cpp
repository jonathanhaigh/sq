#include "ast/ast.h"

#include "gtest/gtest.h"

namespace sq::ast::test {

TEST(AstTest, SingleEntrypoint)
{
    auto root = ast::generate_ast("a");
    EXPECT_EQ(root.data().name(), "");
    EXPECT_EQ(root.children().size(), 1);

    const auto& a = root.children().front();
    EXPECT_EQ(a.data().name(), "a");
    EXPECT_EQ(a.children().size(), 0);
}

TEST(AstTest, DotExpression)
{
    auto root = ast::generate_ast("a.b");
    EXPECT_EQ(root.data().name(), "");
    EXPECT_EQ(root.children().size(), 1);

    const auto& a = root.children().front();
    EXPECT_EQ(a.data().name(), "a");
    EXPECT_EQ(a.children().size(), 1);

    const auto& b = a.children().front();
    EXPECT_EQ(b.data().name(), "b");
    EXPECT_TRUE(b.children().empty());
}

TEST(AstTest, BraceExpression)
{
    auto root = ast::generate_ast("a { b c }");
    EXPECT_EQ(root.data().name(), "");
    EXPECT_EQ(root.children().size(), 1);

    const auto& a = root.children().front();
    EXPECT_EQ(a.data().name(), "a");
    EXPECT_EQ(a.children().size(), 2);

    const auto& b = a.children().front();
    EXPECT_EQ(b.data().name(), "b");
    EXPECT_TRUE(b.children().empty());

    const auto& c = a.children().back();
    EXPECT_EQ(c.data().name(), "c");
    EXPECT_TRUE(c.children().empty());
}

TEST(AstTest, MultipleEntrypoints)
{
    auto root = ast::generate_ast("a b");
    EXPECT_EQ(root.data().name(), "");
    EXPECT_EQ(root.children().size(), 2);

    const auto& a = root.children().front();
    EXPECT_EQ(a.data().name(), "a");
    EXPECT_TRUE(a.children().empty());

    const auto& b = root.children().back();
    EXPECT_EQ(b.data().name(), "b");
    EXPECT_TRUE(b.children().empty());
}

} // namespace sq::ast::test
