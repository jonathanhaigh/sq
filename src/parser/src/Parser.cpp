/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "parser/Parser.h"

#include "common_types/ParseError.h"

#include <cerrno>
#include <cstdlib>
#include <gsl/gsl>
#include <memory>
#include <sstream>

namespace sq::parser {

Parser::Parser(const TokenView &tokens) : tokens_{tokens} {}

Ast Parser::parse() {
  auto ast = Ast{ast_root_node_name, FieldAccessType::Default};
  if (!parse_query(ast)) {
    throw ParseError(tokens_.read(), expecting_);
  }
  return ast;
}

// query: field_tree_list Token::Kind::Eof
bool Parser::parse_query(Ast &parent) {
  if (!parse_field_tree_list(parent)) {
    return false;
  };
  (void)expect_token(Token::Kind::Eof);
  return true;
}

// field_tree_list: field_tree field_tree*
bool Parser::parse_field_tree_list(Ast &parent) {
  if (!parse_field_tree(parent)) {
    return false;
  }
  while (parse_field_tree(parent)) {
  }
  return true;
}

// field_tree: dot_expression brace_expression?
bool Parser::parse_field_tree(Ast &parent) {
  auto *child = parse_dot_expression(parent);
  if (child == nullptr) {
    return false;
  }
  (void)parse_brace_expression(*child);
  return true;
}

// brace_expression: LBrace field_tree_list RBrace
bool Parser::parse_brace_expression(Ast &parent) {
  if (!accept_token(Token::Kind::LBrace)) {
    return false;
  }
  if (!parse_field_tree_list(parent)) {
    throw ParseError(tokens_.read(), expecting_);
  }
  (void)expect_token(Token::Kind::RBrace);
  return true;
}

// dot_expression: field_call (Dot field_call)*
Ast *Parser::parse_dot_expression(Ast &parent) {
  if (!parse_field_call(parent)) {
    return nullptr;
  }
  auto current_parent = gsl::not_null{std::addressof(parent.children().back())};
  while (accept_token(Token::Kind::Dot)) {
    if (!parse_field_call(*current_parent)) {
      throw ParseError(tokens_.read(), expecting_);
    }
    current_parent = std::addressof(current_parent->children().back());
  }
  return current_parent;
}

// field_call: field_access_type? Identifier parameter_pack? list_filter?;
bool Parser::parse_field_call(Ast &parent) {
  const auto fat = parse_field_access_type();
  const auto opt_id = accept_token(Token::Kind::Identifier);
  if (fat == FieldAccessType::Default && !opt_id) {
    return false;
  }
  if (!opt_id) {
    throw ParseError(tokens_.read(), expecting_);
  }
  auto &child = parent.children().emplace_back(opt_id.value().view(), fat);
  (void)parse_parameter_pack(child);
  (void)parse_list_filter(child);
  return true;
}

// field_access_type: Pullup?
FieldAccessType Parser::parse_field_access_type() {
  if (accept_token(Token::Kind::LessThan)) {
    return FieldAccessType::Pullup;
  }
  return FieldAccessType::Default;
}

// parameter_pack: LParen (
//                    param_list_with_pos_params |
//                    param_list_without_pos_params
//                 )? RParen
//
// param_list_with_pos_params: positional_parameter
//                             (Comma positional_parameter)*
//                             (Comma named_parameter)*
//
// param_list_without_pos_params: named_parameter
//                                (Comma named_parameter)*
//
bool Parser::parse_parameter_pack(Ast &parent) {
  if (!accept_token(Token::Kind::LParen)) {
    return false;
  }
  // The code doesn't follow the same shape as the grammar (as written above)
  // here because the grammar gets a bit messy but it's simple to say in code
  // or English: A list of parameters, separated by commas, where parameters
  // can be either positional or named, and all of the positional parameters
  // must come before all of the named parameters.
  auto pos_count = 0;
  auto named_count = 0;
  while (parse_parameter(parent, pos_count, named_count)) {
  }
  (void)expect_token(Token::Kind::RParen);
  return true;
}

// Doesn't represent a symbol in the grammar because it's easier to code this
// way.
// Parses the next parameter in a parameter list, whether it's the first
// argument or not, and whether it's positional or named.
bool Parser::parse_parameter(Ast &parent, int &pos_count, int &named_count) {
  // Require a comma unless we're parsing the first parameter
  if ((pos_count + named_count) > 0 && !accept_token(Token::Kind::Comma)) {
    return false;
  }
  // Only parse a positional parameter if we haven't seen any named ones yet.
  if (named_count == 0 && parse_positional_parameter(parent)) {
    ++pos_count;
    return true;
  }
  if (parse_named_parameter(parent)) {
    ++named_count;
    return true;
  }
  if ((pos_count + named_count) > 0) {
    // We've seen a comma without a parameter after it
    throw ParseError(tokens_.read(), expecting_);
  }
  return false;
}

// positional_parameter: primitive_value
bool Parser::parse_positional_parameter(Ast &parent) {
  if (auto opt_prim = parse_primitive_value()) {
    parent.data().params().pos_params().emplace_back(
        std::move(opt_prim.value()));
    return true;
  }
  return false;
}

// named_parameter: Identifier Equals primitive_value
bool Parser::parse_named_parameter(Ast &parent) {
  const auto opt_id_token = accept_token(Token::Kind::Identifier);
  if (!opt_id_token) {
    return false;
  }
  const auto &id = opt_id_token.value().view();

  (void)expect_token(Token::Kind::Equals);

  auto opt_prim = parse_primitive_value();
  if (!opt_prim) {
    throw ParseError(tokens_.read(), expecting_);
  }
  parent.data().params().named_params().emplace(id, opt_prim.value());
  return true;
}

// primitive_value: (Integer | DQString | bool)
std::optional<Primitive> Parser::parse_primitive_value() {
  if (const auto opt_float = parse_float()) {
    return Primitive{opt_float.value()};
  }
  if (const auto opt_int = parse_integer<PrimitiveInt>()) {
    return Primitive{opt_int.value()};
  }
  if (auto opt_str = parse_dqstring()) {
    return Primitive{std::move(opt_str.value())};
  }
  if (const auto opt_bool = parse_bool()) {
    return Primitive{opt_bool.value()};
  }
  return std::nullopt;
}

// DQString
std::optional<PrimitiveString> Parser::parse_dqstring() {
  const auto opt_token = accept_token(Token::Kind::DQString);
  if (!opt_token) {
    return std::nullopt;
  }
  auto ss = std::istringstream{std::string{opt_token.value().view()}};
  auto ret = PrimitiveString{};
  ss >> quoted(ret);
  return ret;
}

// bool: (BoolTrue | BoolFalse)
std::optional<PrimitiveBool> Parser::parse_bool() {
  if (accept_token(Token::Kind::BoolTrue)) {
    return true;
  }
  if (accept_token(Token::Kind::BoolFalse)) {
    return false;
  }
  return std::nullopt;
}

// Float
std::optional<PrimitiveFloat> Parser::parse_float() {
  // NOTE: ideally we'd turn parse_integer() into a generic parse_number()
  // function that uses std::from_chars for both integers and floats, but gcc
  // doesn't seem to have implemented std::from_chars for floating point
  // types yet.

  const auto opt_token = accept_token(Token::Kind::Float);
  if (!opt_token) {
    return std::nullopt;
  }

  // strtod requires a NUL terminated string so make a std::string copy to
  // make sure we can get that.
  const std::string float_str{opt_token.value().view()};
  const gsl::czstring<> float_cstr{float_str.c_str()};
  gsl::zstring<> str_end{nullptr};
  errno = 0;
  auto ret = std::strtod(float_cstr, &str_end);
  if (errno == ERANGE) {
    auto ss = std::ostringstream{};
    ss << "float " << float_str << " does not fit in required type; "
       << "must be in the closed interval ["
       << std::numeric_limits<PrimitiveFloat>::min() << ", "
       << std::numeric_limits<PrimitiveFloat>::max() << "]";
    throw OutOfRangeError(opt_token.value(), ss.str());
  }
  // We shouldn't get errors other than range errors because the tokenizer
  // has already told us that the token matches the pattern for a float.
  ASSERT(str_end == float_cstr + float_str.size());
  return ret;
}

// list_filter: LBracket (slice_or_element_access | condition) RBracket
bool Parser::parse_list_filter(Ast &parent) {
  if (!accept_token(Token::Kind::LBracket)) {
    return false;
  }
  if (!parse_slice_or_element_access(parent) && !parse_condition(parent)) {
    throw ParseError(tokens_.read(), expecting_);
  }
  (void)expect_token(Token::Kind::RBracket);
  return true;
}

// slice_or_element_access: (
//      Integer |
//      Integer? Colon Integer? (Colon Integer?)?
// )
bool Parser::parse_slice_or_element_access(Ast &parent) {
  // NOTE: parse both slices and element accesses in the same function to
  // avoid backtracking: they cannot be distinguished based on their first
  // tokens - they can both start with an Integer.
  //
  const auto opt_start = parse_integer<gsl::index>();
  const auto opt_colon = accept_token(Token::Kind::Colon);
  if (!opt_start && !opt_colon) {
    return false;
  }
  if (!opt_colon) {
    parent.data().filter_spec() = ElementAccessSpec{opt_start.value()};
    return true;
  }
  const auto opt_stop = parse_integer<gsl::index>();
  const auto opt_colon2 = accept_token(Token::Kind::Colon);
  const auto opt_step = opt_colon2 ? parse_integer<gsl::index>() : std::nullopt;

  parent.data().filter_spec() = SliceSpec{opt_start, opt_stop, opt_step};
  return true;
}

// condition: Identifier? comparison_operator primitive_value
bool Parser::parse_condition(Ast &parent) {

  const auto opt_id = accept_token(Token::Kind::Identifier);
  const auto opt_op = parse_comparison_operator();

  if (!opt_id && !opt_op) {
    return false;
  }
  if (!opt_op) {
    throw ParseError(tokens_.read(), expecting_);
  }
  const auto member = opt_id ? opt_id.value().view() : std::string_view{};

  auto prim = parse_primitive_value();
  if (!prim) {
    throw ParseError(tokens_.read(), expecting_);
  }
  parent.data().filter_spec() = ComparisonSpec{
      std::string{member}, opt_op.value(), std::move(prim.value())};
  return true;
}

// comparison_operator: (
//      GreaterThanOrEqualTo |
//      GreaterThan |
//      LessThanOrEqualTo |
//      LessThan |
//      Equals
// )
std::optional<ComparisonOperator> Parser::parse_comparison_operator() {
  if (accept_token(Token::Kind::GreaterThanOrEqualTo)) {
    return ComparisonOperator::GreaterThanOrEqualTo;
  }
  if (accept_token(Token::Kind::GreaterThan)) {
    return ComparisonOperator::GreaterThan;
  }
  if (accept_token(Token::Kind::LessThanOrEqualTo)) {
    return ComparisonOperator::LessThanOrEqualTo;
  }
  if (accept_token(Token::Kind::LessThan)) {
    return ComparisonOperator::LessThan;
  }
  if (accept_token(Token::Kind::Equals)) {
    return ComparisonOperator::Equals;
  }
  return std::nullopt;
}

void Parser::shift_token() {
  tokens_.next();
  expecting_.clear();
}

std::optional<Token> Parser::accept_token(Token::Kind kind) {
  Expects(ranges::begin(tokens_) != ranges::end(tokens_));
  if (tokens_.read().kind() == kind) {
    auto ret = tokens_.read();
    shift_token();
    return ret;
  }
  expecting_.insert(kind);
  return std::nullopt;
}

Token Parser::expect_token(Token::Kind kind) {
  auto opt_token = accept_token(kind);
  if (!opt_token) {
    throw ParseError(tokens_.read(), expecting_);
  }
  return opt_token.value();
}

} // namespace sq::parser
