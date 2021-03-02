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

#include <concepts>
#include <optional>

namespace sq::parser {

/**
 * Generates an abstract syntax tree (AST) representing the input query from
 * from a stream of tokens.
 */
class Parser
{
public:
    /**
     * Create a Parser that uses the given TokenView as a stream of tokens for
     * the input query.
     */
    explicit Parser(const TokenView& tokens);

    /**
     * Generate an AST for the input query.
     */
    [[nodiscard]] Ast parse();

private:
    [[nodiscard]] bool parse_query(Ast& parent);
    [[nodiscard]] bool parse_field_tree_list(Ast& parent);
    [[nodiscard]] bool parse_field_tree(Ast& parent);
    [[nodiscard]] bool parse_brace_expression(Ast& parent);
    [[nodiscard]] Ast* parse_dot_expression(Ast& parent);
    [[nodiscard]] bool parse_field_call(Ast& parent);
    [[nodiscard]] bool parse_parameter_pack(Ast& parent);
    [[nodiscard]] bool parse_parameter_list(Ast& parent);
    [[nodiscard]] bool parse_parameter(Ast& parent);
    [[nodiscard]] std::optional<Primitive> parse_primitive_value();
    [[nodiscard]] std::optional<PrimitiveString> parse_dqstring();
    [[nodiscard]] std::optional<PrimitiveBool> parse_bool();
    [[nodiscard]] std::optional<PrimitiveFloat> parse_float();
    [[nodiscard]] bool parse_named_parameter(Ast& parent);
    [[nodiscard]] bool parse_list_filter(Ast& parent);
    [[nodiscard]] bool parse_slice_or_element_access(Ast& parent);
    [[nodiscard]] bool parse_condition(Ast& parent);
    [[nodiscard]] std::optional<ComparisonOperator> parse_comparison_operator();

    template <std::integral Int>
    [[nodiscard]] std::optional<Int> parse_integer();

    void shift_token();
    [[nodiscard]] std::optional<Token> accept_token(Token::Kind kind);
    [[nodiscard]] Token expect_token(Token::Kind kind);

    TokenView tokens_;
    Token::KindSet expecting_;
};

} // namespace sq::parser

#include "parser/Parser.inl.h"

#endif // SQ_INCLUDE_GUARD_parser_Parser_h_
