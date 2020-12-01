#include "ast/ast.h"

#include "field_types/Primitive.h"
#include "parser/SqLexer.h"
#include "parser/SqParser.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace sp = sq::parser;
using FieldTreeList = sp::SqParser::Field_tree_listContext;
using FieldTree = sp::SqParser::Field_treeContext;
using DotExpression = sp::SqParser::Dot_expressionContext;
using FieldCall = sp::SqParser::Field_callContext;
using ParameterList = sp::SqParser::Parameter_listContext;
using Parameter = sp::SqParser::ParameterContext;
using NamedParameter = sp::SqParser::Named_parameterContext;
using PrimitiveValue = sp::SqParser::Primitive_valueContext;
using Boolean = sp::SqParser::BooleanContext;
using TerminalNode = antlr4::tree::TerminalNode;

namespace sq::ast {

std::ostream& operator<<(std::ostream& os, const AstData& ast_data)
{
    if (ast_data.name().empty())
    {
        os << "ROOT";
        return os;
    }
    os << ast_data.name() << "(" << ast_data.params() << ")";
    return os;
}

static void parse_field_tree(Ast& parent, FieldTree& ft);
static void parse_field_tree_list(Ast& parent, FieldTreeList& ftl);
static void parse_field_call(Ast& parent, FieldCall& fc);
static void parse_parameters(Ast& parent, ParameterList& pl);
static void parse_parameter(Ast& parent, Parameter& p);
static field_types::Primitive  parse_primitive_value(PrimitiveValue& pl);
static field_types::PrimitiveInt parse_integer(TerminalNode& i);
static field_types::PrimitiveString parse_dq_str(TerminalNode& dq_s);
static field_types::PrimitiveBool parse_boolean(Boolean& b);

static void parse_field_tree_list(Ast& parent, FieldTreeList& ftl)
{
    const auto ft_ptrs = ftl.field_tree();
    assert(ft_ptrs.size());
    for (const auto ft_ptr : ft_ptrs)
    {
        assert(ft_ptr);
        parse_field_tree(parent, *ft_ptr);
    }
}

static void parse_field_tree(Ast& parent, FieldTree& ft)
{
    const auto de_ptr = ft.dot_expression();
    assert(de_ptr);

    auto current_ptr = &parent;
    for (const auto de_fc_ptr : de_ptr->field_call())
    {
        assert(de_fc_ptr);
        parse_field_call(*current_ptr, *de_fc_ptr);
        assert(current_ptr->children().size() == 1);
        current_ptr = &current_ptr->children().front();
    }

    const auto be_ptr = ft.brace_expression();
    if (be_ptr)
    {
        const auto ftl_ptr = be_ptr->field_tree_list();
        assert(ftl_ptr);
        parse_field_tree_list(*current_ptr, *ftl_ptr);
    }
}

static void parse_field_call(Ast& parent, FieldCall& fc)
{
    const auto id_ptr = fc.ID();
    assert(id_ptr);
    auto& fc_ast = parent.children().emplace_back(id_ptr->getText());

    const auto pl_ptr = fc.parameter_list();
    if (pl_ptr)
    {
        parse_parameters(fc_ast, *pl_ptr);
    }
}

static void parse_parameters(Ast& parent, ParameterList& pl)
{
    for (const auto p_ptr : pl.parameter())
    {
        assert(p_ptr);
        parse_parameter(parent, *p_ptr);
    }
}

static void parse_parameter(Ast& parent, Parameter& p)
{
    if (p.primitive_value())
    {
        parent.data().params().pos_params().push_back(
            parse_primitive_value(*p.primitive_value())
        );
        return;
    }
    const auto np_ptr = p.named_parameter();
    assert(np_ptr);
    assert(np_ptr->ID());
    assert(np_ptr->primitive_value());
    const auto [it, success] = parent.data().params().named_params().emplace(
        np_ptr->ID()->getText(),
        parse_primitive_value(*(np_ptr->primitive_value()))
    );
}

static field_types::Primitive parse_primitive_value(PrimitiveValue& pl)
{
    if (pl.INTEGER())
    {
        return parse_integer(*(pl.INTEGER()));
    }
    else if (pl.DQ_STR())
    {
        return parse_dq_str(*(pl.DQ_STR()));
    }
    assert(pl.boolean());
    return parse_boolean(*(pl.boolean()));
}

static field_types::PrimitiveInt parse_integer(TerminalNode& i)
{
    auto ss = std::stringstream{i.getText()};
    auto ret = field_types::PrimitiveInt{};
    // TODO: better error checking... Or use something like boost::numeric_cast
    ss >> ret;
    return ret;
}

static field_types::PrimitiveString parse_dq_str(TerminalNode& dq_s)
{
    auto ss = std::stringstream{dq_s.getText()};
    auto ret = field_types::PrimitiveString{};
    ss >> quoted(ret);
    return ret;
}

static field_types::PrimitiveBool parse_boolean(Boolean& b)
{
    if (b.BOOLEAN_TRUE())
    {
        return true;
    }
    assert(b.BOOLEAN_FALSE());
    return false;
}

Ast generate_ast(const std::string& sq_command)
{
    auto input_stream = antlr4::ANTLRInputStream{sq_command};
    auto lexer = sp::SqLexer{&input_stream};
    auto token_stream = antlr4::CommonTokenStream{&lexer};
    auto parser = sp::SqParser{&token_stream};
    const auto ftl_ptr = parser.field_tree_list();
    assert(ftl_ptr);
    auto root = Ast{""};
    parse_field_tree_list(root, *ftl_ptr);
    return root;
}

} // namespace sq::ast
