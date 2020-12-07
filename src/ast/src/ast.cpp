#include "ast/ast.h"

#include "ast/ParseError.h"
#include "field_types/Primitive.h"
#include "parser/SqLexer.h"
#include "parser/SqParser.h"

#include <cassert>
#include <charconv>
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
    assert(!ft_ptrs.empty());
    for (const auto ft_ptr : ft_ptrs)
    {
        assert(ft_ptr != nullptr);
        parse_field_tree(parent, *ft_ptr);
    }
}

static void parse_field_tree(Ast& parent, FieldTree& ft)
{
    const auto de_ptr = ft.dot_expression();
    assert(de_ptr != nullptr);

    auto current_ptr = &parent;
    for (const auto de_fc_ptr : de_ptr->field_call())
    {
        assert(de_fc_ptr != nullptr);
        parse_field_call(*current_ptr, *de_fc_ptr);
        assert(current_ptr->children().size() == 1);
        current_ptr = &current_ptr->children().front();
    }

    const auto be_ptr = ft.brace_expression();
    if (be_ptr != nullptr)
    {
        const auto ftl_ptr = be_ptr->field_tree_list();
        assert(ftl_ptr != nullptr);
        parse_field_tree_list(*current_ptr, *ftl_ptr);
    }
}

static void parse_field_call(Ast& parent, FieldCall& fc)
{
    const auto id_ptr = fc.ID();
    assert(id_ptr != nullptr);
    auto& fc_ast = parent.children().emplace_back(id_ptr->getText());

    const auto pl_ptr = fc.parameter_list();
    if (pl_ptr != nullptr)
    {
        parse_parameters(fc_ast, *pl_ptr);
    }
}

static void parse_parameters(Ast& parent, ParameterList& pl)
{
    for (const auto p_ptr : pl.parameter())
    {
        assert(p_ptr != nullptr);
        parse_parameter(parent, *p_ptr);
    }
}

static void parse_parameter(Ast& parent, Parameter& p)
{
    if (p.primitive_value() != nullptr)
    {
        parent.data().params().pos_params().push_back(
            parse_primitive_value(*p.primitive_value())
        );
        return;
    }
    const auto np_ptr = p.named_parameter();
    assert(np_ptr != nullptr);
    assert(np_ptr->ID() != nullptr);
    assert(np_ptr->primitive_value() != nullptr);
    const auto [it, success] = parent.data().params().named_params().emplace(
        np_ptr->ID()->getText(),
        parse_primitive_value(*(np_ptr->primitive_value()))
    );
    if (!success)
    {
        auto ss = std::ostringstream{};
        ss << "named parameter \"" << np_ptr->ID()->getText()
           << "\" given multiple times in one call";
        throw SqParseError(np_ptr->ID()->getSymbol(), ss.str());
    }
}

static field_types::Primitive parse_primitive_value(PrimitiveValue& pl)
{
    if (pl.INTEGER() != nullptr)
    {
        return parse_integer(*(pl.INTEGER()));
    }
    if (pl.DQ_STR() != nullptr)
    {
        return parse_dq_str(*(pl.DQ_STR()));
    }
    assert(pl.boolean() != nullptr);
    return parse_boolean(*(pl.boolean()));
}

static field_types::PrimitiveInt parse_integer(TerminalNode& i)
{
    const auto str = i.getText();
    const auto begin = str.data();
    // We can't really avoid using pointer arithmetic when using
    // std::from_chars - it requires a const char* to indicate the end of the
    // string, but we can only reasonably make one using pointer arithmetic.
    // In particular, std::string, std::string_view, std::span etc. only
    // provide iterator versions of end(), but we need a pointer.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    const auto end = begin + str.size();
    field_types::PrimitiveInt value = 0;
    const auto [ptr, ec] = std::from_chars(begin, end, value, 10);
    if (ec == std::errc::result_out_of_range)
    {
        auto ss = std::ostringstream{};
        ss << "integer \"" << str << "\" is out of range";
        throw SqParseError(i.getSymbol(), ss.str());
    }
    if (ec != std::errc() || ptr != end)
    {
        auto ss = std::ostringstream{};
        ss << "failed to parse \"" << str << "\" as integer";
        throw SqParseError(i.getSymbol(), ss.str());
    }
    return value;
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
    if (b.BOOLEAN_TRUE() != nullptr)
    {
        return true;
    }
    assert(b.BOOLEAN_FALSE() != nullptr);
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
