#include "ast/ast.h"

#include "field_types/Primitive.h"
#include "parser/SqLexer.h"
#include "parser/SqParser.h"
#include "util/strutil.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace sp = sq::parser;
using FieldTreeList = sp::SqParser::Field_tree_listContext;
using FieldTree = sp::SqParser::Field_treeContext;
using DotExpression = sp::SqParser::Dot_expressionContext;
using FieldCall = sp::SqParser::Field_callContext;
using ParameterList = sp::SqParser::Parameter_listContext;
using PrimitiveValue = sp::SqParser::Primitive_valueContext;
using TerminalNode = antlr4::tree::TerminalNode;

namespace sq::ast {

std::ostream& operator<<(std::ostream& os, const AstData& ast_data)
{
    if (ast_data.name().empty())
    {
        os << "ROOT";
        return os;
    }
    // TODO: Something better... Especially when we have ranges or
    // std::ostream_joiner
    const auto& params = ast_data.params().pos_params();
    auto param_strs = std::vector<std::string>{};
    std::transform(
        std::begin(params),
        std::end(params),
        std::back_inserter(param_strs),
        [](const auto& param) { return util::variant_to_str(param); }
    );
    os << ast_data.name() << "(" << util::join(param_strs) << ")";
    return os;
}

static void parse_field_tree(Ast& parent, FieldTree& ft);
static void parse_field_tree_list(Ast& parent, FieldTreeList& ftl);
static void parse_field_call(Ast& parent, FieldCall& fc);
static void parse_positional_parameters(Ast& parent, ParameterList& pl);
static field_types::Primitive  parse_primitive_value(PrimitiveValue& pl);
static field_types::PrimitiveInt parse_integer(TerminalNode& i);
static field_types::PrimitiveString parse_dq_str(TerminalNode& dq_s);

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
        parse_positional_parameters(fc_ast, *pl_ptr);
    }
}

static void parse_positional_parameters(Ast& parent, ParameterList& pl)
{
    for (const auto pv_ptr : pl.primitive_value())
    {
        assert(pv_ptr);
        parent.data().params().pos_params().push_back(parse_primitive_value(*pv_ptr));
    }
}

static field_types::Primitive parse_primitive_value(PrimitiveValue& pl)
{
    if (pl.INTEGER())
    {
        return parse_integer(*(pl.INTEGER()));
    }
    assert(pl.DQ_STR());
    return parse_dq_str(*(pl.DQ_STR()));
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
