#include "results/results.h"

#include "field_types/SqRoot.gen.h"

#include <cassert>
#include <gsl/narrow>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_access.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/slice.hpp>
#include <range/v3/view/stride.hpp>

using namespace std::string_literals;

namespace sq::results {

using FieldPtr = field_types::FieldPtr;
using FieldList = field_types::FieldList;
using FieldInputRange = field_types::FieldInputRange;
using Primitive = field_types::Primitive;
using Data = ResultTree::Data;
using ObjData = ResultTree::ObjData;
using ArrayData = ResultTree::ArrayData;

static FieldPtr range_at(FieldList&& rng, ptrdiff_t index)
{
    auto positive_index = index;
    if (index < 0)
    {
        auto size = gsl::narrow<ptrdiff_t>(rng.size());
        positive_index = size + index;
        if (positive_index < 0)
        {
            throw std::out_of_range("range_at<FieldList>: out of range");
        }
    }
    return std::move(rng.at(gsl::narrow<FieldList::size_type>(positive_index)));
}

static FieldPtr range_at(FieldInputRange&& rng, ptrdiff_t index)
{
    if (index < 0)
    {
        // index < 0 means the index is relative to the end of the range, but
        // we only have an input range so we won't know where the end is until
        // we get there. We'll have to save the elements of the range in a
        // container and then work out the element specified by the index.
        return range_at(rng | ranges::to<std::vector>(), index);
    }
    auto it = ranges::begin(rng);
    const auto end = ranges::end(rng);
    auto i = index;
    while (i != 0 && it != end)
    {
        --i;
        ++it;
    }
    if (it == end)
    {
        throw std::out_of_range("range_at<FieldInputRange>: out of range");
    }
    return *it;
}

static ptrdiff_t normalize_range_index(ptrdiff_t index, ptrdiff_t range_size)
{
    if (index < 0)
    {
        return range_size + index;
    }
    return index;
}

template <typename R>
static ArrayData create_array_data(const ast::Ast& ast, R&& rng)
{
    auto arr = ArrayData{};
    for (auto&& result : rng)
    {
        arr.emplace_back(ast, std::move(result));
    }
    return arr;
}

static Data data_from_slice(
    const ast::Ast& ast,
    const ast::ListSliceSpec& spec,
    FieldList&& list_result)
{
    // The range-v3 library has a slice function but it's not quite enough
    // to emulate Python's slices directly. Specifically:
    // * range-v3's slice doesn't have a step/stride parameter. range-v3
    //   does have a "stride" view, but it doesn't support negative
    //   strides.
    // * range-v3's slice supports negative indexes, but only by providing
    //   overloads with different signatures, which means it's not ideal
    //   for our use here.
    // We therefore will:
    // * Convert negative indeces into positive ones.
    // * Make adjustments for negative steps/strides. 

    const auto size = gsl::narrow<ptrdiff_t>(list_result.size());
    const auto step = spec.step_.value_or(1);
    assert(step != 0);

    if (step > 0)
    {
        const auto start = normalize_range_index(spec.start_.value_or(0), size);
        const auto stop = normalize_range_index(spec.stop_.value_or(size), size);
        if (start > stop) { return ArrayData{}; }
        auto rng = list_result |
               ranges::views::slice(start, stop) |
               ranges::views::stride(step);
        return create_array_data(ast, rng);
    }

    const auto stop = 1 + normalize_range_index(spec.start_.value_or(size - 1), size);
    auto start = std::ptrdiff_t{0};
    if (spec.stop_)
    {
        start = 1 + normalize_range_index(spec.stop_.value(), size);
    }
    if (start > stop) { return ArrayData{}; }
    auto rng = list_result |
           ranges::views::slice(start, stop) |
           ranges::views::reverse |
           ranges::views::stride(-step);
    return create_array_data(ast, rng);
}

static Data data_from_slice(
    const ast::Ast& ast,
    const ast::ListSliceSpec& spec,
    FieldInputRange&& range_result)
{
    const auto start = spec.start_.value_or(0);
    const auto step = spec.step_.value_or(1);
    if (start < 0 || spec.stop_ || step < 0)
    {
        // We can't handle these cases for an input range so we'll have to
        // save the data to a FieldList and work with that.
        return data_from_slice(ast, spec, range_result | ranges::to<std::vector>());
    }
    auto rng = range_result |
           ranges::views::drop(start) |
           ranges::views::stride(step);
    return create_array_data(ast, std::move(rng));
}



class ResultToDataVisitor
{
public:
    explicit ResultToDataVisitor(const ast::Ast& ast)
        : ast_{&ast}
    { }

    Data operator()(FieldPtr&& field_ptr) const;
    Data operator()(FieldList&& field_list) const;
    Data operator()(FieldInputRange&& field_range) const;

private:
    const ast::Ast* ast_;
};

Data ResultToDataVisitor::operator()(FieldPtr&& field_ptr) const
{
    if (ast_->children().empty())
    {
        return field_ptr->to_primitive();
    }

    auto obj = ObjData{};
    for (const auto& child : ast_->children())
    {
        const auto& field_name = child.data().name();
        const auto& params = child.data().params();
        const auto visitor = ResultToDataVisitor{child};
        obj.emplace_back(
            field_name,
            ResultTree{
                child,
                std::visit(visitor, field_ptr->get(field_name, params))
            }
        );
    }
    return obj;
}

Data ResultToDataVisitor::operator()(FieldList&& field_list) const
{
    const auto& filter_spec = ast_->data().list_filter_spec();

    if (std::holds_alternative<ast::ListElementAccessSpec>(filter_spec))
    {
        const auto index = std::get<ast::ListElementAccessSpec>(filter_spec).index_;
        // TODO: the range_at call below could throw std::out_of_range. When
        // that happens we should catch and rethrow with the position in the
        // query that contained the index. Will need to augment the AST
        // structure to contain that info.
        return (*this)(range_at(std::move(field_list), index));
    }

    if (std::holds_alternative<ast::ListSliceSpec>(filter_spec))
    {
        const auto spec = std::get<ast::ListSliceSpec>(filter_spec);
        return data_from_slice(*ast_, spec, std::move(field_list));
    }

    assert(std::holds_alternative<ast::NoListFilterSpec>(filter_spec));
    return create_array_data(*ast_, std::move(field_list));
}

Data ResultToDataVisitor::operator()(FieldInputRange&& field_range) const
{
    const auto& filter_spec = ast_->data().list_filter_spec();

    if (std::holds_alternative<ast::ListElementAccessSpec>(filter_spec))
    {
        const auto index = std::get<ast::ListElementAccessSpec>(filter_spec).index_;
        // TODO: the range_at call below could throw std::out_of_range. When
        // that happens we should catch and rethrow with the position in the
        // query that contained the index. Will need to augment the AST
        // structure to contain that info.
        return (*this)(range_at(std::move(field_range), index));
    }

    if (std::holds_alternative<ast::ListSliceSpec>(filter_spec))
    {
        const auto& spec = std::get<ast::ListSliceSpec>(filter_spec);
        return data_from_slice(*ast_, spec, std::move(field_range));
    }

    assert(std::holds_alternative<ast::NoListFilterSpec>(filter_spec));
    return create_array_data(*ast_, std::move(field_range));
}


ResultTree::ResultTree(const ast::Ast& ast)
    : ResultTree{ast, field_types::SqRoot::create()}
{ }

ResultTree::ResultTree(const ast::Ast& ast, field_types::Result&& result)
    : ResultTree{ast, std::visit(ResultToDataVisitor{ast}, std::move(result))}
{ }

ResultTree::ResultTree(const ast::Ast& ast, Data&& data)
    : ast_{&ast}
    , data_{std::move(data)}
{ }


ResultTree generate_results(const ast::Ast& ast)
{
    return ResultTree{ast};
}

} // namespace sq::results
