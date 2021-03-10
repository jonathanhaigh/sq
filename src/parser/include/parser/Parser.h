/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_parser_Parser_h_
#define SQ_INCLUDE_GUARD_parser_Parser_h_

#include "common_types/Primitive.h"
#include "common_types/Token.h"
#include "parser/Ast.h"
#include "parser/TokenView.h"
#include "util/typeutil.h"

#include <concepts>
#include <optional>

namespace sq::parser {

/**
 * Generates an abstract syntax tree (AST) representing the input query from
 * from a stream of tokens.
 */
class Parser {
public:
  /**
   * Create a Parser that uses the given TokenView as a stream of tokens for
   * the input query.
   */
  explicit Parser(const TokenView &tokens);

  /**
   * Generate an AST for the input query.
   */
  SQ_ND Ast parse();

private:
  SQ_ND bool parse_query(Ast &parent);
  SQ_ND bool parse_field_tree_list(Ast &parent);
  SQ_ND bool parse_field_tree(Ast &parent);
  SQ_ND bool parse_brace_expression(Ast &parent);
  SQ_ND Ast *parse_dot_expression(Ast &parent);
  SQ_ND bool parse_field_call(Ast &parent);
  SQ_ND FieldAccessType parse_field_access_type();
  SQ_ND bool parse_parameter_pack(Ast &parent);
  SQ_ND bool parse_parameter(Ast &parent, int &pos_count, int &named_count);
  SQ_ND bool parse_positional_parameter(Ast &parent);
  SQ_ND bool parse_named_parameter(Ast &parent);
  SQ_ND std::optional<Primitive> parse_primitive_value();
  SQ_ND std::optional<PrimitiveString> parse_dqstring();
  SQ_ND std::optional<PrimitiveBool> parse_bool();
  SQ_ND std::optional<PrimitiveFloat> parse_float();
  SQ_ND bool parse_list_filter(Ast &parent);
  SQ_ND bool parse_slice_or_element_access(Ast &parent);
  SQ_ND bool parse_condition(Ast &parent);
  SQ_ND std::optional<ComparisonOperator> parse_comparison_operator();

  template <std::integral Int> SQ_ND std::optional<Int> parse_integer();

  void shift_token();
  SQ_ND std::optional<Token> accept_token(Token::Kind kind);
  SQ_ND Token expect_token(Token::Kind kind);

  TokenView tokens_;
  Token::KindSet expecting_;
};

} // namespace sq::parser

#include "parser/Parser.inl.h"

#endif // SQ_INCLUDE_GUARD_parser_Parser_h_
