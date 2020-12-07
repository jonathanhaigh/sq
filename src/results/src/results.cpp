#include "results/results.h"

#include "field_types/SqRoot.gen.h"
#include "field_types/SqRootImpl.h"

#include <cassert>

using namespace std::string_literals;

namespace sq::results {

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

ResultTree::ResultTree(const ast::Ast& ast)
    : ResultTree(ast, field_types::SqRoot::create())
{ }

ResultTree::ResultTree(const ast::Ast& ast, field_types::FieldList&& list_result)
    : ast_{&ast}
    , data_{std::in_place_type_t<ArrayData>()}
{
    auto& arr = std::get<ArrayData>(data_);

    for (auto&& result : list_result)
    {
        arr.emplace_back(ast, std::move(result));
    }
}

ResultTree::ResultTree(const ast::Ast& ast, field_types::FieldPtr&& result)
    :ast_{&ast}
    , data_{std::in_place_type_t<ObjData>()}
{
    if (ast.children().empty())
    {
        data_ = result->to_primitive();
        return;
    }

    auto& obj = std::get<ObjData>(data_);

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
}

ResultTree::ResultTree(const ast::Ast& ast, field_types::Primitive&& result)
    : ast_{&ast}
    , data_{result}
{ }

ResultTree generate_results(const ast::Ast& ast)
{
    return ResultTree{ast};
}

} // namespace sq::results
