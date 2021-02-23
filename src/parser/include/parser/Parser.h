#ifndef SQ_INCLUDE_GUARD_parser_Parser_h_
#define SQ_INCLUDE_GUARD_parser_Parser_h_

#include "common_types/Primitive.h"
#include "common_types/Token.h"
#include "parser/Ast.h"
#include "parser/TokenView.h"

#include <concepts>
#include <optional>

namespace sq::parser {

class Parser
{
public:
    explicit Parser(const TokenView& tokens);
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
    [[nodiscard]] bool parse_named_parameter(Ast& parent);
    [[nodiscard]] bool parse_list_filter(Ast& parent);

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
