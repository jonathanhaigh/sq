/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "parser/Parser.h"

#include "common_types/ParseError.h"

#include <gsl/gsl>
#include <memory>
#include <sstream>

namespace sq::parser {

Parser::Parser(const TokenView& tokens)
    : tokens_{tokens}
{ }

Ast Parser::parse()
{
    auto ast = Ast{ast_root_node_name};
    if (!parse_query(ast))
    {
        throw ParseError(tokens_.read(), expecting_);
    }
    return ast;
}

// query: field_tree_list Token::Kind::Eof
bool Parser::parse_query(Ast& parent)
{
    if (!parse_field_tree_list(parent))
    {
        return false;
    };
    (void)expect_token(Token::Kind::Eof);
    return true;
}

// field_tree_list: field_tree field_tree*
bool Parser::parse_field_tree_list(Ast& parent)
{
    if (!parse_field_tree(parent))
    {
        return false;
    }
    while(parse_field_tree(parent))
    { }
    return true;
}

// field_tree: dot_expression brace_expression?
bool Parser::parse_field_tree(Ast& parent)
{
    auto* child = parse_dot_expression(parent);
    if (child == nullptr)
    {
        return false;
    }
    (void)parse_brace_expression(*child);
    return true;
}

// brace_expression: LBrace field_tree_list RBrace
bool Parser::parse_brace_expression(Ast& parent)
{
    if (!accept_token(Token::Kind::LBrace))
    {
        return false;
    }
    if (!parse_field_tree_list(parent))
    {
        throw ParseError(tokens_.read(), expecting_);
    }
    (void)expect_token(Token::Kind::RBrace);
    return true;
}

// dot_expression: field_call (Dot field_call)*
Ast* Parser::parse_dot_expression(Ast& parent)
{
    if (!parse_field_call(parent))
    {
        return nullptr;
    }
    auto current_parent = gsl::not_null{std::addressof(parent.children().back())};
    while (accept_token(Token::Kind::Dot))
    {
        if (!parse_field_call(*current_parent))
        {
            throw ParseError(tokens_.read(), expecting_);
        }
        current_parent = std::addressof(current_parent->children().back());
    }
    return current_parent;
}

// field_call: Identifier parameter_list? list_filter?;
bool Parser::parse_field_call(Ast& parent)
{
    const auto opt_id = accept_token(Token::Kind::Identifier);
    if (!opt_id)
    {
        return false;
    }
    auto &child = parent.children().emplace_back(opt_id.value().view());
    (void)parse_parameter_pack(child);
    (void)parse_list_filter(child);
    return true;
}

// parameter_pack: LParen parameter_list? Rparen
bool Parser::parse_parameter_pack(Ast& parent)
{
    if (!accept_token(Token::Kind::LParen))
    {
        return false;
    }
    (void)parse_parameter_list(parent);
    (void)expect_token(Token::Kind::RParen);
    return true;
}

// parameter_list: parameter (Comma parameter)*
bool Parser::parse_parameter_list(Ast& parent)
{
    if (!parse_parameter(parent))
    {
        return false;
    }
    while (accept_token(Token::Kind::Comma))
    {
        if (!parse_parameter(parent))
        {
            throw ParseError(tokens_.read(), expecting_);
        }
    }
    return true;
}

// parameter: (primitive_value | named_parameter)
bool Parser::parse_parameter(Ast& parent)
{
    if (auto opt_prim = parse_primitive_value())
    {
        parent.data().params().pos_params().emplace_back(
            std::move(opt_prim.value())
        );
        return true;
    }
    return parse_named_parameter(parent);
}

// primitive_value: (Integer | DQString | bool)
std::optional<Primitive> Parser::parse_primitive_value()
{
    if (const auto opt_int = parse_integer<PrimitiveInt>())
    {
        return Primitive{opt_int.value()};
    }
    if (auto opt_str = parse_dqstring())
    {
        return Primitive{std::move(opt_str.value())};
    }
    if (const auto opt_bool = parse_bool())
    {
        return Primitive{opt_bool.value()};
    }
    return std::nullopt;
}

// DQString
std::optional<PrimitiveString> Parser::parse_dqstring()
{
    const auto opt_token = accept_token(Token::Kind::DQString);
    if (!opt_token)
    {
        return std::nullopt;
    }
    auto ss = std::istringstream{std::string{opt_token.value().view()}};
    auto ret = PrimitiveString{};
    ss >> quoted(ret);
    return ret;
}

// bool: (BoolTrue | BoolFalse)
std::optional<PrimitiveBool> Parser::parse_bool()
{
    if (accept_token(Token::Kind::BoolTrue))
    {
        return true;
    }
    if (accept_token(Token::Kind::BoolFalse))
    {
        return false;
    }
    return std::nullopt;
}

// named_parameter: Identifier Equals primitive_value
bool Parser::parse_named_parameter(Ast& parent)
{
    const auto opt_id_token = accept_token(Token::Kind::Identifier);
    if (!opt_id_token)
    {
        return false;
    }
    const auto& id = opt_id_token.value().view();

    (void)expect_token(Token::Kind::Equals);

    auto opt_prim = parse_primitive_value();
    if (!opt_prim)
    {
        throw ParseError(tokens_.read(), expecting_);
    }
    parent.data().params().named_params().emplace(id, opt_prim.value());
    return true;
}

// list_filter: (
//                  LBracket Integer RBracket |
//                  LBracket Integer? Colon Integer? (Colon Integer?)? RBracket
//              )
bool Parser::parse_list_filter(Ast& parent)
{
    if (!accept_token(Token::Kind::LBracket))
    {
        return false;
    }
    const auto opt_start = parse_integer<gsl::index>();
    const auto opt_colon = accept_token(Token::Kind::Colon);
    if (opt_start && !opt_colon)
    {
        parent.data().filter_spec() = ElementAccessSpec{opt_start.value()};
    }
    else if (opt_colon)
    {
        const auto opt_stop = parse_integer<gsl::index>();
        const auto opt_colon2 = accept_token(Token::Kind::Colon);
        const auto opt_step = opt_colon2?
            parse_integer<gsl::index>() :
            std::nullopt;

        parent.data().filter_spec() = SliceSpec{opt_start, opt_stop, opt_step};
    }
    else
    {
        throw ParseError(tokens_.read(), expecting_);
    }
    (void)expect_token(Token::Kind::RBracket);
    return true;
}

void Parser::shift_token()
{
    tokens_.next();
    expecting_.clear();
}

std::optional<Token> Parser::accept_token(Token::Kind kind)
{
    Expects(ranges::begin(tokens_) != ranges::end(tokens_));
    if (tokens_.read().kind() == kind)
    {
        auto ret = tokens_.read();
        shift_token();
        return ret;
    }
    expecting_.insert(kind);
    return std::nullopt;
}

Token Parser::expect_token(Token::Kind kind)
{
    Expects(ranges::begin(tokens_) != ranges::end(tokens_));
    if (tokens_.read().kind() != kind)
    {
        throw ParseError(tokens_.read(), Token::KindSet{kind});
    }
    auto ret = tokens_.read();
    shift_token();
    return ret;
}

} // namespace sq::parser
