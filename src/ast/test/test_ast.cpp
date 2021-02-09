#include "ast/ast.h"
#include "ast/FilterSpec.h"
#include "util/strutil.h"


#include <gtest/gtest.h>

namespace sq::ast::test {

using Size = Ast::Children::size_type;

static const auto no_params = FieldCallParams{};
static const auto no_filter_spec = FilterSpec{NoFilterSpec{}};

void expect_node(
    const Ast& node,
    const char* const name,
    const FieldCallParams& params,
    const FilterSpec& filter_spec,
    const Size noof_children
)
{
    auto ss = std::ostringstream{};
    ss << "expect_node(node, "
        << std::quoted(name) << ", "
        << params << ", "
        << util::variant_to_str(filter_spec) << ", "
        << noof_children
        << ")";
    SCOPED_TRACE(ss.str());
    EXPECT_EQ(node.data().name(), name);
    EXPECT_EQ(node.data().params(), params);
    EXPECT_EQ(node.data().filter_spec(), filter_spec);
    EXPECT_EQ(node.children().size(), noof_children);
}

void expect_plain_node(const Ast& node, const char*const name, const Size noof_children)
{
    SCOPED_TRACE("expect_plain_node");
    expect_node(node, name, no_params, no_filter_spec, noof_children);
}

void expect_plain_leaf(const Ast& node, const char*const name)
{
    SCOPED_TRACE("expect_plain_leaf");
    expect_plain_node(node, name, 0);
}

void expect_root(const Ast& node, const Size noof_children)
{
    SCOPED_TRACE("expect_root");
    return expect_plain_node(node, "", noof_children);
}

void expect_equivalent_query(const char* const q1, const char* const q2)
{
    auto ss = std::ostringstream{};
    ss << "expect_equivalent_query(" << q1 << ", " << q2 << ")";
    SCOPED_TRACE(ss.str());
    ASSERT_EQ(generate_ast(q1), generate_ast(q2));
}

TEST(AstTest, DotExpression)
{
    const auto root = generate_ast("a.b");
    expect_root(root, 1);

    const auto& a = root.children().front();
    expect_plain_node(a, "a", 1);

    const auto& b = a.children().front();
    expect_plain_leaf(b, "b");
}

TEST(AstTest, BraceExpression)
{
    expect_equivalent_query("a { b }", "a.b");
    expect_equivalent_query("a { b { c } }", "a.b.c");
    expect_equivalent_query("a { b { c { d } } }", "a.b.c.d");

    const auto root = generate_ast("a { b c }");
    expect_root(root, 1);

    const auto& a = root.children().front();
    expect_plain_node(a, "a", 2);

    const auto& b = a.children().front();
    expect_plain_leaf(b, "b");

    const auto& c = a.children().back();
    expect_plain_leaf(c, "c");
}

TEST(AstTest, MultipleEntrypoints)
{
    auto root = generate_ast("a b");
    expect_root(root, 2);

    const auto& a = root.children().front();
    expect_plain_leaf(a, "a");

    const auto& b = root.children().back();
    expect_plain_leaf(b, "b");
}

using SimpleTestCase = std::tuple<const char*, FieldCallParams, FilterSpec>;
class SimpleAstTest : public testing::TestWithParam<SimpleTestCase> { };

TEST_P(SimpleAstTest, SimpleTest)
{
    const auto [query, params, filter_spec] = GetParam();

    auto root = generate_ast(query);
    expect_root(root, 1);

    const auto& a = root.children().front();
    expect_node(a, "a", params, filter_spec, 0);
}

template <typename T>
Primitive to_primitive(T&& v)
{
    return std::forward<T>(v);
}

Primitive to_primitive(int v)
{
    return static_cast<PrimitiveInt>(v);
}

Primitive to_primitive(const char* v)
{
    return PrimitiveString(v);
}

template <typename... Args>
static FieldCallParams pos_params(Args... args)
{
    auto params = FieldCallParams{};
    params.pos_params() = std::initializer_list<Primitive>{to_primitive(std::forward<Args>(args))...};
    return params;
}

INSTANTIATE_TEST_SUITE_P(
    SimpleAstTestInstantiation,
    SimpleAstTest,
    testing::Values(
        SimpleTestCase{"a", no_params, no_filter_spec},
        SimpleTestCase{"a[0]", no_params, ElementAccessSpec{0}},
        SimpleTestCase{"a[99]", no_params, ElementAccessSpec{99}},
        SimpleTestCase{"a[-1]", no_params, ElementAccessSpec{-1}},
        SimpleTestCase{"a[-50]", no_params, ElementAccessSpec{-50}},
        SimpleTestCase{"a[:]", no_params, SliceSpec{std::nullopt, std::nullopt, std::nullopt}},
        SimpleTestCase{"a[1:]", no_params, SliceSpec{1, std::nullopt, std::nullopt}},
        SimpleTestCase{"a[:1]", no_params, SliceSpec{std::nullopt, 1, std::nullopt}},
        SimpleTestCase{"a[1:1]", no_params, SliceSpec{1, 1, std::nullopt}},
        SimpleTestCase{"a[::]", no_params, SliceSpec{std::nullopt, std::nullopt, std::nullopt}},
        SimpleTestCase{"a[1::]", no_params, SliceSpec{1, std::nullopt, std::nullopt}},
        SimpleTestCase{"a[:1:]", no_params, SliceSpec{std::nullopt, 1, std::nullopt}},
        SimpleTestCase{"a[1:1:]", no_params, SliceSpec{1, 1, std::nullopt}},
        SimpleTestCase{"a[::1]", no_params, SliceSpec{std::nullopt, std::nullopt, 1}},
        SimpleTestCase{"a[1::1]", no_params, SliceSpec{1, std::nullopt, 1}},
        SimpleTestCase{"a[:1:1]", no_params, SliceSpec{std::nullopt, 1, 1}},
        SimpleTestCase{"a[1:1:1]", no_params, SliceSpec{1, 1, 1}},
        SimpleTestCase{"a(1)", pos_params(1), no_filter_spec},
        SimpleTestCase{"a(-1)", pos_params(-1), no_filter_spec},
        SimpleTestCase{"a(\"str\")", pos_params("str"), no_filter_spec},
        SimpleTestCase{"a(true)", pos_params(true), no_filter_spec},
        SimpleTestCase{"a(false)", pos_params(false), no_filter_spec},
        SimpleTestCase{"a(1, 2, 3, 4)", pos_params(1, 2, 3, 4), no_filter_spec}
    )
);

} // namespace sq::ast::test
