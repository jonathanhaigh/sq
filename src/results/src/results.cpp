#include "results/results.h"

#include "field_types/SqRoot.gen.h"
#include "results/Filter.h"

#include <iostream>

using namespace std::string_literals;

namespace sq::results {

using Data = ResultTree::Data;
using ObjData = ResultTree::ObjData;
using ArrayData = ResultTree::ArrayData;

class ResultViewToDataVisitor
{
public:
    explicit ResultViewToDataVisitor(const ast::Ast& ast)
        : ast_{&ast}
    { }

    Data operator()(FieldPtr&& field) const;

    template <ranges::category Cat> 
    Data operator()(FieldRange<Cat>&& rng) const;

private:
    const ast::Ast* ast_;
};

Data ResultViewToDataVisitor::operator()(FieldPtr&& field) const
{
    if (ast_->children().empty())
    {
        return field->to_primitive();
    }

    auto obj = ObjData{};
    for (const auto& child : ast_->children())
    {
        const auto& field_name = child.data().name();
        const auto& params = child.data().params();
        const auto visitor = ResultViewToDataVisitor{child};
        const auto filter = Filter::create(child.data().filter_spec());
        auto child_result = filter->transform_result_for_requirements(
            field->get(field_name, params)
        );
        auto child_result_view = get_result_view(child_result);
        obj.emplace_back(
            field_name,
            ResultTree{
                child,
                std::visit(visitor, filter->view(std::move(child_result_view)))
            }
        );
    }
    return obj;
}

template <ranges::category Cat>
Data ResultViewToDataVisitor::operator()(FieldRange<Cat>&& rng) const
{
    auto arr = ArrayData{};
    for (auto field : rng)
    {
        arr.emplace_back(*ast_, std::move(field));
    }
    return arr;
}

ResultTree::ResultTree(const ast::Ast& ast, ResultView&& result_view)
    : ResultTree{ast, std::visit(ResultViewToDataVisitor{ast}, std::move(result_view))}
{ }

ResultTree::ResultTree(const ast::Ast& ast, Data&& data)
    : ast_{&ast}
    , data_{std::move(data)}
{ }

ResultTree generate_results(const ast::Ast& ast)
{
    return ResultTree{ast, field_types::SqRoot::create()};
}

} // namespace sq::results
