/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/ParseError.h"
#include "parser/Ast.h"
#include "parser/FilterSpec.h"
#include "parser/Parser.h"
#include "parser/TokenView.h"
#include "test/FieldCallParams_test_util.h"
#include "util/strutil.h"

#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <string_view>
#include <utility>

namespace sq::test {
namespace {

using namespace sq::parser;
using Size = Ast::Children::size_type;

inline constexpr auto no_filter_spec = NoFilterSpec{};

parser::Ast generate_ast(std::string_view query) {
  auto tokens = parser::TokenView{query};
  auto parser = parser::Parser{tokens};
  return parser.parse();
}

void expect_node(const Ast &node, std::string_view name,
                 const FieldCallParams &params, const FilterSpec &filter_spec,
                 Size noof_children) {
  SCOPED_TRACE(testing::Message()
               << "expect_node(node, " << std::quoted(name) << ", " << params
               << ", " << util::variant_to_str(filter_spec) << ", "
               << noof_children << ")");
  EXPECT_EQ(node.data().name(), name);
  EXPECT_EQ(node.data().params(), params);
  EXPECT_EQ(node.data().filter_spec(), filter_spec);
  EXPECT_EQ(node.children().size(), noof_children);
}

void expect_plain_node(const Ast &node, std::string_view name,
                       Size noof_children) {
  SCOPED_TRACE("expect_plain_node");
  expect_node(node, name, FieldCallParams{}, no_filter_spec, noof_children);
}

void expect_plain_leaf(const Ast &node, std::string_view name) {
  SCOPED_TRACE("expect_plain_leaf");
  expect_plain_node(node, name, 0);
}

void expect_root(const Ast &node, Size noof_children) {
  SCOPED_TRACE("expect_root");
  return expect_plain_node(node, ast_root_node_name, noof_children);
}

void expect_equivalent_query(std::string_view q1, std::string_view q2) {
  SCOPED_TRACE(testing::Message()
               << "expect_equivalent_query(" << q1 << ", " << q2 << ")");
  ASSERT_EQ(generate_ast(q1), generate_ast(q2));
}

TEST(AstTest, DotExpression) {
  const auto root = generate_ast("a.b");
  expect_root(root, 1);

  const auto &a = root.children().front();
  expect_plain_node(a, "a", 1);

  const auto &b = a.children().front();
  expect_plain_leaf(b, "b");
}

TEST(AstTest, EquivalentQueries) {
  expect_equivalent_query("a { b }", "a.b");
  expect_equivalent_query("a { b { c } }", "a.b.c");
  expect_equivalent_query("a { b { c { d } } }", "a.b.c.d");
}

TEST(AstTest, BraceExpression) {
  const auto root = generate_ast("a { b c }");
  expect_root(root, 1);

  const auto &a = root.children().front();
  expect_plain_node(a, "a", 2);

  const auto &b = a.children().front();
  expect_plain_leaf(b, "b");

  const auto &c = a.children().back();
  expect_plain_leaf(c, "c");
}

TEST(AstTest, MultipleEntrypoints) {
  auto root = generate_ast("a b");
  expect_root(root, 2);

  const auto &a = root.children().front();
  expect_plain_leaf(a, "a");

  const auto &b = root.children().back();
  expect_plain_leaf(b, "b");
}

struct SimpleTestCase {
  std::string_view query_;
  FieldCallParams fcp_;
  FilterSpec filter_spec_;
};

std::ostream &operator<<(std::ostream &os, const SimpleTestCase &tc) {
  os << "(query=" << tc.query_ << ";fcp=" << tc.fcp_
     << ";filter=" << util::variant_to_str(tc.filter_spec_) << ")";
  return os;
}

class SimpleAstTest : public testing::TestWithParam<SimpleTestCase> {};

TEST_P(SimpleAstTest, SimpleTest) {
  const auto [query, params, filter_spec] = GetParam();

  auto root = generate_ast(query);
  expect_root(root, 1);

  const auto &a = root.children().front();
  expect_node(a, "a", params, filter_spec, 0);
}

INSTANTIATE_TEST_SUITE_P(
    SimpleAstTestInstantiation, SimpleAstTest,
    testing::Values(
        SimpleTestCase{"a", FieldCallParams{}, no_filter_spec},
        SimpleTestCase{"a[0]", FieldCallParams{}, ElementAccessSpec{0}},
        SimpleTestCase{"a[99]", FieldCallParams{}, ElementAccessSpec{99}},
        SimpleTestCase{"a[-1]", FieldCallParams{}, ElementAccessSpec{-1}},
        SimpleTestCase{"a[-50]", FieldCallParams{}, ElementAccessSpec{-50}},
        SimpleTestCase{"a[:]", FieldCallParams{},
                       SliceSpec{std::nullopt, std::nullopt, std::nullopt}},
        SimpleTestCase{"a[1:]", FieldCallParams{},
                       SliceSpec{1, std::nullopt, std::nullopt}},
        SimpleTestCase{"a[:1]", FieldCallParams{},
                       SliceSpec{std::nullopt, 1, std::nullopt}},
        SimpleTestCase{"a[1:1]", FieldCallParams{},
                       SliceSpec{1, 1, std::nullopt}},
        SimpleTestCase{"a[::]", FieldCallParams{},
                       SliceSpec{std::nullopt, std::nullopt, std::nullopt}},
        SimpleTestCase{"a[1::]", FieldCallParams{},
                       SliceSpec{1, std::nullopt, std::nullopt}},
        SimpleTestCase{"a[:1:]", FieldCallParams{},
                       SliceSpec{std::nullopt, 1, std::nullopt}},
        SimpleTestCase{"a[1:1:]", FieldCallParams{},
                       SliceSpec{1, 1, std::nullopt}},
        SimpleTestCase{"a[::1]", FieldCallParams{},
                       SliceSpec{std::nullopt, std::nullopt, 1}},
        SimpleTestCase{"a[1::1]", FieldCallParams{},
                       SliceSpec{1, std::nullopt, 1}},
        SimpleTestCase{"a[:1:1]", FieldCallParams{},
                       SliceSpec{std::nullopt, 1, 1}},
        SimpleTestCase{"a[1:1:1]", FieldCallParams{}, SliceSpec{1, 1, 1}},
        SimpleTestCase{"a[=true]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::Equals, true}},
        SimpleTestCase{"a[=false]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::Equals, false}},
        SimpleTestCase{"a[=\"str\"]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::Equals,
                                      to_primitive("str")}},
        SimpleTestCase{
            "a[=-10]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::Equals, to_primitive(-10)}},
        SimpleTestCase{
            "a[=99]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::Equals, to_primitive(99)}},
        SimpleTestCase{"a[=-10.2]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::Equals,
                                      to_primitive(-10.2)}},
        SimpleTestCase{
            "a[=99.2]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::Equals, to_primitive(99.2)}},
        SimpleTestCase{
            "a[>=true]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::GreaterThanOrEqualTo, true}},
        SimpleTestCase{"a[>=false]", FieldCallParams{},
                       ComparisonSpec{"",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      false}},
        SimpleTestCase{"a[>=\"str\"]", FieldCallParams{},
                       ComparisonSpec{"",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive("str")}},
        SimpleTestCase{"a[>=-10]", FieldCallParams{},
                       ComparisonSpec{"",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive(-10)}},
        SimpleTestCase{"a[>=99]", FieldCallParams{},
                       ComparisonSpec{"",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive(99)}},
        SimpleTestCase{"a[>=-10.2]", FieldCallParams{},
                       ComparisonSpec{"",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[>=99.2]", FieldCallParams{},
                       ComparisonSpec{"",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive(99.2)}},
        SimpleTestCase{
            "a[>true]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::GreaterThan, true}},
        SimpleTestCase{
            "a[>false]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::GreaterThan, false}},
        SimpleTestCase{"a[>\"str\"]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::GreaterThan,
                                      to_primitive("str")}},
        SimpleTestCase{"a[>-10]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::GreaterThan,
                                      to_primitive(-10)}},
        SimpleTestCase{"a[>99]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::GreaterThan,
                                      to_primitive(99)}},
        SimpleTestCase{"a[>-10.2]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::GreaterThan,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[>99.2]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::GreaterThan,
                                      to_primitive(99.2)}},
        SimpleTestCase{
            "a[<=true]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::LessThanOrEqualTo, true}},
        SimpleTestCase{
            "a[<=false]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::LessThanOrEqualTo, false}},
        SimpleTestCase{"a[<=\"str\"]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive("str")}},
        SimpleTestCase{"a[<=-10]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive(-10)}},
        SimpleTestCase{"a[<=99]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive(99)}},
        SimpleTestCase{"a[<=-10.2]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[<=99.2]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive(99.2)}},
        SimpleTestCase{"a[<true]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThan, true}},
        SimpleTestCase{"a[<false]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThan, false}},
        SimpleTestCase{"a[<\"str\"]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThan,
                                      to_primitive("str")}},
        SimpleTestCase{"a[<-10]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThan,
                                      to_primitive(-10)}},
        SimpleTestCase{
            "a[<99]", FieldCallParams{},
            ComparisonSpec{"", ComparisonOperator::LessThan, to_primitive(99)}},
        SimpleTestCase{"a[<-10.2]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThan,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[<99.2]", FieldCallParams{},
                       ComparisonSpec{"", ComparisonOperator::LessThan,
                                      to_primitive(99.2)}},
        SimpleTestCase{
            "a[member=true]", FieldCallParams{},
            ComparisonSpec{"member", ComparisonOperator::Equals, true}},
        SimpleTestCase{
            "a[member=false]", FieldCallParams{},
            ComparisonSpec{"member", ComparisonOperator::Equals, false}},
        SimpleTestCase{"a[member=\"str\"]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::Equals,
                                      to_primitive("str")}},
        SimpleTestCase{"a[member=-10]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::Equals,
                                      to_primitive(-10)}},
        SimpleTestCase{"a[member=99]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::Equals,
                                      to_primitive(99)}},
        SimpleTestCase{"a[member=-10.2]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::Equals,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[member=99.2]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::Equals,
                                      to_primitive(99.2)}},
        SimpleTestCase{"a[member>=true]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      true}},
        SimpleTestCase{"a[member>=false]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      false}},
        SimpleTestCase{"a[member>=\"str\"]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive("str")}},
        SimpleTestCase{"a[member>=-10]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive(-10)}},
        SimpleTestCase{"a[member>=99]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive(99)}},
        SimpleTestCase{"a[member>=-10.2]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[member>=99.2]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::GreaterThanOrEqualTo,
                                      to_primitive(99.2)}},
        SimpleTestCase{
            "a[member>true]", FieldCallParams{},
            ComparisonSpec{"member", ComparisonOperator::GreaterThan, true}},
        SimpleTestCase{
            "a[member>false]", FieldCallParams{},
            ComparisonSpec{"member", ComparisonOperator::GreaterThan, false}},
        SimpleTestCase{"a[member>\"str\"]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::GreaterThan,
                                      to_primitive("str")}},
        SimpleTestCase{"a[member>-10]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::GreaterThan,
                                      to_primitive(-10)}},
        SimpleTestCase{"a[member>99]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::GreaterThan,
                                      to_primitive(99)}},
        SimpleTestCase{"a[member>-10.2]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::GreaterThan,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[member>99.2]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::GreaterThan,
                                      to_primitive(99.2)}},
        SimpleTestCase{"a[member<=true]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::LessThanOrEqualTo,
                                      true}},
        SimpleTestCase{"a[member<=false]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::LessThanOrEqualTo,
                                      false}},
        SimpleTestCase{"a[member<=\"str\"]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive("str")}},
        SimpleTestCase{"a[member<=-10]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive(-10)}},
        SimpleTestCase{"a[member<=99]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive(99)}},
        SimpleTestCase{"a[member<=-10.2]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[member<=99.2]", FieldCallParams{},
                       ComparisonSpec{"member",
                                      ComparisonOperator::LessThanOrEqualTo,
                                      to_primitive(99.2)}},
        SimpleTestCase{
            "a[member<true]", FieldCallParams{},
            ComparisonSpec{"member", ComparisonOperator::LessThan, true}},
        SimpleTestCase{
            "a[member<false]", FieldCallParams{},
            ComparisonSpec{"member", ComparisonOperator::LessThan, false}},
        SimpleTestCase{"a[member<\"str\"]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::LessThan,
                                      to_primitive("str")}},
        SimpleTestCase{"a[member<-10]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::LessThan,
                                      to_primitive(-10)}},
        SimpleTestCase{"a[member<99]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::LessThan,
                                      to_primitive(99)}},
        SimpleTestCase{"a[member<-10.2]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::LessThan,
                                      to_primitive(-10.2)}},
        SimpleTestCase{"a[member<99.2]", FieldCallParams{},
                       ComparisonSpec{"member", ComparisonOperator::LessThan,
                                      to_primitive(99.2)}},
        SimpleTestCase{"a(1)", params(1), no_filter_spec},
        SimpleTestCase{"a(-1)", params(-1), no_filter_spec},
        SimpleTestCase{"a(9223372036854775807)",
                       params(std::numeric_limits<PrimitiveInt>::max()),
                       no_filter_spec},
        SimpleTestCase{"a(-9223372036854775808)",
                       params(std::numeric_limits<PrimitiveInt>::min()),
                       no_filter_spec},
        SimpleTestCase{"a(1.)", params(1.), no_filter_spec},
        SimpleTestCase{"a(-1.)", params(-1.), no_filter_spec},
        SimpleTestCase{"a(1.0)", params(1.0), no_filter_spec},
        SimpleTestCase{"a(-1.0)", params(-1.0), no_filter_spec},
        SimpleTestCase{"a(0.0)", params(0.0), no_filter_spec},
        SimpleTestCase{"a(-0.0)", params(-0.0), no_filter_spec},
        SimpleTestCase{"a(0.)", params(0.), no_filter_spec},
        SimpleTestCase{"a(-0.)", params(-0.), no_filter_spec},
        SimpleTestCase{"a(.0)", params(.0), no_filter_spec},
        SimpleTestCase{"a(-.0)", params(-.0), no_filter_spec},
        SimpleTestCase{"a(\"str\")", params("str"), no_filter_spec},
        SimpleTestCase{"a(true)", params(true), no_filter_spec},
        SimpleTestCase{"a(false)", params(false), no_filter_spec},
        SimpleTestCase{"a(1, 2, 3, 4)", params(1, 2, 3, 4), no_filter_spec},
        SimpleTestCase{"a(n=1)", params(named("n", 1)), no_filter_spec},
        SimpleTestCase{"a(n=-1)", params(named("n", -1)), no_filter_spec},
        SimpleTestCase{"a(n=\"str\")", params(named("n", "str")),
                       no_filter_spec},
        SimpleTestCase{"a(n=true)", params(named("n", true)), no_filter_spec},
        SimpleTestCase{"a(n=false)", params(named("n", false)), no_filter_spec},
        SimpleTestCase{
            "a(n=1, o=2, p=3, q=4)",
            params(named("n", 1), named("o", 2), named("p", 3), named("q", 4)),
            no_filter_spec},
        SimpleTestCase{"a(1, true, \"str\", n=9)",
                       params(1, true, "str", named("n", 9)), no_filter_spec},
        SimpleTestCase{"a(true_n=1)", params(named("true_n", 1)),
                       no_filter_spec},
        SimpleTestCase{"a(false_n=1)", params(named("false_n", 1)),
                       no_filter_spec}));

class InvalidQueryTest : public testing::TestWithParam<const char *> {};

TEST_P(InvalidQueryTest, InvalidQuery) {
  try {
    const auto ast = generate_ast(GetParam());
    FAIL() << "Expected ParseError; ast = " << ast;
  } catch (const ParseError &e) {
  }
}

INSTANTIATE_TEST_SUITE_P(InvalidQueryTestInstantiation, InvalidQueryTest,
                         testing::Values(".a", "a.", "{", "a {", "}", "a }",
                                         "a { }", "{ }", "{ } a", "{ } a", "a(",
                                         "a)", "a(1", "a(\"str)", "a(p=)",
                                         "a(p)", "a(p=1,2)", "a(p-x)", "a[]",
                                         "a[=]", "a[10=]", "a[1.0]", "a[1.0:]",
                                         "a[:1.0:]", "a[::1.0]", "a(\")"));

} // namespace
} // namespace sq::test
