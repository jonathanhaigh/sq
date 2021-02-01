#include "results/results.h"

#include "exceptions/NotImplementedError.h"
#include "field_types/SqRoot.gen.h"
#include "field_types/SqRootImpl.h"

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

static ptrdiff_t normalize_range_index(ptrdiff_t index, ptrdiff_t range_size)
{
    if (index < 0)
    {
        return range_size + index;
    }
    return index;
}

template <typename R>
static ResultTree::ArrayData create_array_data(const ast::Ast& ast, R&& rng)
{
    auto arr = ResultTree::ArrayData{};
    for (auto&& result : rng)
    {
        arr.emplace_back(ast, std::move(result));
    }
    return arr;
}

static ResultTree::Data data_from_field_list(
    const ast::Ast& ast,
    FieldList&& list_result)
{
    const auto& filter_spec = ast.data().list_filter_spec();

    if (std::holds_alternative<ast::ListElementAccessSpec>(filter_spec))
    {
        const auto index = std::get<ast::ListElementAccessSpec>(filter_spec).index_;
        return data_from_field_ptr(ast, range_at(std::move(list_result), index));
    }

    if (std::holds_alternative<ast::ListSliceSpec>(filter_spec))
    {
        const auto& spec = std::get<ast::ListSliceSpec>(filter_spec);
        const auto size = gsl::narrow<ptrdiff_t>(list_result.size());
        const auto step = spec.step_.value_or(1);
        assert(step != 0);
        const bool reverse = step < 0;

        const auto start = normalize_range_index(
            spec.start_.value_or(reverse? size : 0),
            size
        );

        const auto stop = normalize_range_index(
            spec.stop_.value_or(reverse? 0 : size),
            size
        );

        if (reverse)
        {
            if (start < stop) { return ResultTree::ArrayData{}; }
            auto rng = list_result |
                   ranges::views::reverse |
                   ranges::views::slice(size - start, size - stop) |
                   ranges::views::stride(-step);

            return create_array_data(ast, rng);
        }
        if (start > stop) { return ResultTree::ArrayData{}; }
        auto rng = list_result |
               ranges::views::slice(start, stop) |
               ranges::views::stride(step);
        return create_array_data(ast, rng);
    }

    assert(std::holds_alternative<ast::NoListFilterSpec>(filter_spec));
    return create_array_data(ast, std::move(list_result));
}

static ResultTree::Data data_from_field_input_range(
    const ast::Ast& ast,
    FieldInputRange&& range_result)
{
    const auto& filter_spec = ast.data().list_filter_spec();

    if (std::holds_alternative<ast::ListElementAccessSpec>(filter_spec))
    {
        const auto index = std::get<ast::ListElementAccessSpec>(filter_spec).index_;
        // TODO: the range_at call below could throw std::out_of_range. When
        // that happens we should catch and rethrow with the position in the
        // query that contained the index. Will need to augment the AST
        // structure to contain that info.
        return data_from_field_ptr(ast, range_at(std::move(range_result), index));
    }

    if (std::holds_alternative<ast::ListSliceSpec>(filter_spec))
    {
        const auto& spec = std::get<ast::ListSliceSpec>(filter_spec);
        const auto start = spec.start_.value_or(0);
        const auto step = spec.step_.value_or(1);
        if (start < 0 || spec.stop_ || step < 0)
        {
            // We can't handle these cases for an input range so we'll have to
            // save the data to a vector to get a random access range
            return data_from_field_list(ast, range_result | ranges::to<std::vector>());
        }
        auto rng = range_result |
               ranges::views::drop(start) |
               ranges::views::stride(step);
        return create_array_data(ast, std::move(rng));
    }

    assert(std::holds_alternative<ast::NoListFilterSpec>(filter_spec));
    return create_array_data(ast, std::move(range_result));
}

ResultTree::ResultTree(const ast::Ast& ast)
    : ResultTree(ast, field_types::SqRoot::create())
{ }

ResultTree::ResultTree(const ast::Ast& ast, FieldList&& list_result)
    : ast_{&ast}
    , data_{data_from_field_list(ast, std::move(list_result))}
{
}

ResultTree::ResultTree(const ast::Ast& ast, field_types::FieldInputRange&& result)
    : ast_{&ast}
    , data_{data_from_field_input_range(ast, std::move(result))}
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
