/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/results.h"

#include "results/Filter.h"
#include "util/typeutil.h"

#include <gsl/gsl>

namespace sq::results {

namespace {

using Data = ResultTree::Data;
using ObjData = ResultTree::ObjData;
using ArrayData = ResultTree::ArrayData;

/**
 * Convert the result of accessing a field into a ResultTree containing data
 * ready for serialization and output.
 */
class ResultToDataVisitor
{
public:
    explicit ResultToDataVisitor(const parser::Ast& ast)
        : ast_{&ast}
    { }

    SQ_ND Data operator()(FieldPtr&& field) const;
    SQ_ND Data operator()(ranges::cpp20::view auto&& rng) const;

private:
    gsl::not_null<const parser::Ast*> ast_;
};

Data ResultToDataVisitor::operator()(FieldPtr&& field) const
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
        const auto visitor = ResultToDataVisitor{child};
        const auto filter = Filter::create(child.data().filter_spec());
        auto child_results = (*filter)(field->get(field_name, params));
        obj.emplace_back(
            field_name,
            ResultTree{std::visit(visitor, std::move(child_results))}
        );
    }
    return obj;
}

Data ResultToDataVisitor::operator()(ranges::cpp20::view auto&& rng) const
{
    auto arr = ArrayData{};
    for (auto field : SQ_FWD(rng))
    {
        arr.emplace_back(*ast_, std::move(field));
    }
    return arr;
}

} // namespace

ResultTree::ResultTree(const parser::Ast& ast, Result&& result)
    : ResultTree{std::visit(ResultToDataVisitor{ast}, std::move(result))}
{ }

ResultTree::ResultTree(Data&& data)
    : data_{std::move(data)}
{ }

bool operator==(const ResultTree& lhs, const ResultTree& rhs)
{
    return lhs.data() == rhs.data();
}

bool operator!=(const ResultTree& lhs, const ResultTree& rhs)
{
    return !(lhs == rhs);
}

} // namespace sq::results
