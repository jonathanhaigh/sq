#include "results/results.h"

#include "exceptions/NotImplementedError.h"
#include "field_types/SqRoot.gen.h"
#include "field_types/SqRootImpl.h"

#include <cassert>
#include <gsl/narrow>

using namespace std::string_literals;

namespace sq::results {

using FieldList = field_types::FieldList;
using FieldPtr = field_types::FieldPtr;
using Primitive = field_types::Primitive;

class ResultToResultTreeVisitor
{
public:
    explicit ResultToResultTreeVisitor(const ast::Ast& ast)
        : ast_{&ast}
    { }

    template <typename ResultType>
    ResultTree operator()(ResultType&& result) const
    {
        return ResultTree(*ast_, std::forward<ResultType>(result));
    }

private:
    const ast::Ast* ast_;
};

static ResultTree::Data data_from_field_ptr(
    const ast::Ast& ast,
    FieldPtr&& result)
{
    if (ast.children().empty())
    {
        return result->to_primitive();
    }

    auto obj = ResultTree::ObjData{};
    for (const auto& child : ast.children())
    {
        const auto& field_name = child.data().name();
        const auto& params = child.data().params();
        const auto visitor = ResultToResultTreeVisitor(child);
        obj.emplace_back(
            field_name,
            std::visit(visitor, result->get(field_name, params))
        );
    }
    return obj;
}

static ResultTree::Data data_from_field_list(
    const ast::Ast& ast,
    FieldList&& list_result)
{
    const auto& filter_spec = ast.data().list_filter_spec();

    if (std::holds_alternative<ast::ListElementAccessSpec>(filter_spec))
    {
        const auto index = gsl::narrow<FieldList::size_type>(
            std::get<ast::ListElementAccessSpec>(filter_spec).index_
        );
        return data_from_field_ptr(ast, std::move(list_result.at(index)));
    }

    if (std::holds_alternative<ast::ListSliceSpec>(filter_spec))
    {
        throw NotImplementedError("List slicing has not been implemented");
    }

    assert(std::holds_alternative<ast::NoListFilterSpec>(filter_spec));
    auto arr = ResultTree::ArrayData{};
    for (auto&& result : list_result)
    {
        arr.emplace_back(ast, std::move(result));
    }
    return arr;
}

ResultTree::ResultTree(const ast::Ast& ast)
    : ResultTree(ast, field_types::SqRoot::create())
{ }

ResultTree::ResultTree(const ast::Ast& ast, FieldList&& list_result)
    : ast_{&ast}
    , data_{data_from_field_list(ast, std::move(list_result))}
{
}

ResultTree::ResultTree(const ast::Ast& ast, FieldPtr&& result)
    :ast_{&ast}
    , data_{data_from_field_ptr(ast, std::move(result))}
{
}

ResultTree::ResultTree(const ast::Ast& ast, Primitive&& result)
    : ast_{&ast}
    , data_{std::move(result)}
{ }

ResultTree generate_results(const ast::Ast& ast)
{
    return ResultTree{ast};
}

} // namespace sq::results
