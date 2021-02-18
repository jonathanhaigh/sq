#include "results/Filter.h"

#include "common_types/InternalError.h"
#include "common_types/NotAnArrayError.h"
#include "common_types/OutOfRangeError.h"
#include "util/ASSERT.h"
#include "util/typeutil.h"

#include <cstddef>
#include <gsl/narrow>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_access.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/stride.hpp>
#include <range/v3/view/take.hpp>
#include <sstream>

namespace sq::results {

namespace {

using Int = std::ptrdiff_t;

struct HasRangeCategory
{
    explicit HasRangeCategory(ranges::category cat)
        : cat_{cat}
    { }

    [[nodiscard]] bool operator()(const Result& result) const
    {
        return std::visit(*this, result);
    }

    [[nodiscard]] bool operator()([[maybe_unused]] const FieldPtr& fp) const
    {
        return false;
    }

    template <typename R>
    [[nodiscard]] bool operator()([[maybe_unused]] const R& rng) const
    {
        return (ranges::get_categories<R>() & cat_) == cat_;
    }

private:
    ranges::category cat_;
};

struct SlurpRangeIntoVector
{
    [[nodiscard]] FieldVector operator()(Result&& result) const
    {
        return std::visit(*this, std::move(result));
    }

    [[nodiscard]] FieldVector operator()([[maybe_unused]] const FieldPtr& fp) const
    {
        throw NotAnArrayError("Cannot apply array filter to non-array field");
    }

    template <ranges::category Cat>
    [[nodiscard]] FieldVector operator()(FieldRange<Cat>&& rng) const
    {
        return std::move(rng) | ranges::views::move | ranges::to<std::vector>;
    }

    [[nodiscard]] FieldVector operator()(FieldVector&& vec) const
    {
        return std::move(vec);
    }
};
constexpr auto slurp_range_into_vector = SlurpRangeIntoVector{};

template <typename R>
[[nodiscard]] Int get_range_size(R& rng)
{
    if constexpr (!ranges::forward_range<R> && !ranges::sized_range<R>)
    {
        throw InternalError{
            "get_range_size: internal error: bad range category"
        };
        return 0;
    }
    else
    {
        return gsl::narrow<Int>(ranges::distance(rng));
    }
}

template <typename R, typename = util::disable_lvalues_t<R>>
[[nodiscard]] auto get_reversed_range(R&& rng)
{
    if constexpr (!ranges::bidirectional_range<R>)
    {
        throw InternalError{
            "get_reversed_range: internal error: bad range category"
        };
        return std::forward<R>(rng);
    }
    else
    {
        return std::forward<R>(rng) | ranges::views::reverse;
    }
}

template <typename Spec>
struct FilterImpl;

template <>
struct FilterImpl<ast::NoFilterSpec>
    : Filter
{
    explicit FilterImpl([[maybe_unused]] const ast::NoFilterSpec& spec)
    { }

    [[nodiscard]] Result transform_result_for_requirements(Result&& result) const override
    {
        return std::move(result);
    }

    [[nodiscard]] ResultView view(ResultView&& result) const override
    {
        return std::move(result);
    }
};

template <>
struct FilterImpl<ast::ElementAccessSpec>
    : Filter
{
    explicit FilterImpl(ast::ElementAccessSpec spec)
        : index_{spec.index_}
    { }

    [[nodiscard]] Result transform_result_for_requirements(Result&& result) const override
    {
        if (
            index_ >= 0 ||
            HasRangeCategory{ranges::category::sized}(result) ||
            HasRangeCategory{ranges::category::forward}(result)
        )
        {
            return std::move(result);
        }
        return slurp_range_into_vector(std::move(result));
    }

    [[nodiscard]] ResultView view(ResultView&& result) const override
    {
        return std::visit(*this, std::move(result));
    }

    [[nodiscard]] ResultView operator()([[maybe_unused]] FieldPtr&& fp) const
    {
        throw NotAnArrayError("Cannot apply array filter to non-array field");
    }

    template <ranges::category Cat>
    [[nodiscard]] ResultView operator()(FieldRange<Cat>&& rng) const
    {
        if (index_ >= 0)
        {
            return nonnegative_index_access(std::move(rng), index_);
        }
        const auto size = get_range_size(rng);
        return negative_index_access(std::move(rng), index_, size);
    }

private:

    template <ranges::category Cat>
    [[nodiscard]] ResultView nonnegative_index_access(FieldRange<Cat>&& rng, Int index) const
    {
        ASSERT(index >= 0);
        auto it = ranges::begin(rng);
        auto end = ranges::end(rng);
        ranges::advance(it, index, end);
        if (it == end)
        {
            std::ostringstream ss;
            ss << "array element access (\"[" << index_ << "]\"): out of range";
            if constexpr (Cat != ranges::category::input)
            {
                ss << "(size=" << get_range_size(rng) << ")";
            }
            // clang-tidy bug: https://reviews.llvm.org/D72333
            // NOLINTNEXTLINE(readability-misleading-indentation)
            throw OutOfRangeError(ss.str());
        }
        return std::move(*it);
    }

    template <ranges::category Cat>
    [[nodiscard]] ResultView negative_index_access(FieldRange<Cat>&& rng, Int index, Int size) const
    {
        ASSERT(index < 0);
        auto nonneg_index = size + index;
        if (nonneg_index < 0)
        {
            std::ostringstream ss;
            ss << "array element access (\"[" << index_ << "]\"): out of range (size=" << size << ")";
            throw OutOfRangeError(ss.str());
        }
        return nonnegative_index_access(std::move(rng), nonneg_index);
    }

    Int index_;
};

template <>
struct FilterImpl<ast::SliceSpec>
    : Filter
{
    explicit FilterImpl(ast::SliceSpec spec)
        : start_{spec.start_}
        , stop_{spec.stop_}
        , step_{spec.step_}
    { }

    [[nodiscard]] Result transform_result_for_requirements(Result&& result) const override
    {
        auto step = step_.value_or(1);
        if (step < 0)
        {
            if (HasRangeCategory{ranges::category::bidirectional}(result))
            {
                return std::move(result);
            }
            return slurp_range_into_vector(std::move(result));
        }
        if (start_.value_or(0) < 0 || stop_.value_or(0) < 0)
        {
            if (HasRangeCategory{ranges::category::forward}(result) ||
                HasRangeCategory{ranges::category::sized}(result))
            {
                return std::move(result);
            }
            return slurp_range_into_vector(std::move(result));
        }
        return std::move(result);
    }

    [[nodiscard]] ResultView view(ResultView&& result) const override
    {
        return std::visit(*this, std::move(result));
    }

    [[nodiscard]] ResultView operator()([[maybe_unused]] const FieldPtr& fp) const
    {
        throw NotAnArrayError("Cannot apply array filter to non-array field");
    }

    template <ranges::category Cat>
    [[nodiscard]] ResultView operator()(FieldRange<Cat>&& rng) const
    {
        auto step = step_.value_or(1);
        if (step > 0)
        {
            auto start = start_.value_or(0);
            auto stop = stop_.value_or(0);
            if (stop_) {
                if (start < 0 || stop < 0)
                {
                    return to_result_view(mixed_index_pos_step(std::move(rng), start, stop, step));
                }
                return to_result_view(pos_index_pos_step(std::move(rng), start, stop, step));
            }
            if (start < 0)
            {
                return to_result_view(neg_index_no_stop_pos_step(std::move(rng), start, step));
            }
            return to_result_view(pos_index_no_stop_pos_step(std::move(rng), start, step));
        }

        auto start = start_.value_or(-1);
        auto stop = stop_.value_or(-1);
        if (stop_) {
            if (start >= 0 && stop >= 0)
            {
                return to_result_view(pos_index_neg_step(std::move(rng), start, stop, step));
            }
            if (start >= 0 || stop >= 0)
            {
                return to_result_view(mixed_index_neg_step(std::move(rng), start, stop, step));
            }
            return to_result_view(neg_index_neg_step(std::move(rng), start, stop, step));
        }
        if (start >= 0)
        {
            return to_result_view(pos_index_no_stop_neg_step(std::move(rng), start, step));
        }
        return to_result_view(neg_index_no_stop_neg_step(std::move(rng), start, step));
    }

private:

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static ResultView to_result_view(R&& rng)
    {
        return FieldRange<ranges::get_categories<R>()>(std::forward<R>(rng));
    }


    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto pos_index_pos_step(R&& rng, Int start, Int stop, Int step)
    {
        ASSERT(step > 0);
        ASSERT(start >= 0);
        ASSERT(stop >= 0);
        if (stop < start) { stop = start; }
        return std::forward<R>(rng) |
               ranges::views::drop(start) |
               ranges::views::take(stop - start) |
               ranges::views::stride(step);
    }

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto pos_index_no_stop_pos_step(R&& rng, Int start, Int step)
    {
        ASSERT(start >= 0);
        ASSERT(step > 0);
        return std::forward<R>(rng) |
               ranges::views::drop(start) |
               ranges::views::stride(step);
    }

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto mixed_index_pos_step(R&& rng, Int start, Int stop, Int step)
    {
        ASSERT(ranges::forward_range<R> || ranges::sized_range<R>);
        ASSERT(step > 0);
        ASSERT(start < 0 || stop < 0);
        auto size = get_range_size(rng);

        if (start < 0) { start += size; };
        if (start < 0){ start = 0; }
        if (stop < 0) { stop += size; }
        if (stop < 0) { stop = start; }
        return pos_index_pos_step(std::forward<R>(rng), start, stop, step);
    }

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto neg_index_no_stop_pos_step(R&& rng, Int start, Int step)
    {
        ASSERT(ranges::forward_range<R> || ranges::sized_range<R>);
        ASSERT(step > 0);
        ASSERT(start < 0);
        start += get_range_size(rng);
        if (start < 0) { start = 0; }
        return pos_index_no_stop_pos_step(std::forward<R>(rng), start, step);
    }

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto neg_index_neg_step(R&& rng, Int start, Int stop, Int step)
    {
        ASSERT(ranges::bidirectional_range<R>);
        ASSERT(step < 0);
        ASSERT(start < 0);
        ASSERT(stop < 0);
        start = -start - 1;
        stop = -stop - 1;
        step *= -1;
        return pos_index_pos_step(get_reversed_range(std::forward<R>(rng)), start, stop, step);
    }

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto neg_index_no_stop_neg_step(R&& rng, Int start, Int step)
    {
        ASSERT(ranges::bidirectional_range<R>);
        ASSERT(step < 0);
        ASSERT(start < 0);
        start = -start -1;
        step *= -1;
        return pos_index_no_stop_pos_step(get_reversed_range(std::forward<R>(rng)), start, step);
    }

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto mixed_index_neg_step(R&& rng, Int start, Int stop, Int step)
    {
        ASSERT(ranges::bidirectional_range<R>);
        ASSERT(step < 0);
        ASSERT(start >= 0 || stop >= 0);
        auto size = get_range_size(rng);

        if (start >= 0) { start -= size; };
        if (start >= 0){ start = -1; }
        if (stop >= 0) { stop -= size; }
        if (stop >= 0) { stop = start; }
        return neg_index_neg_step(std::forward<R>(rng), start, stop, step);
    }

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto pos_index_neg_step(R&& rng, Int start, Int stop, Int step)
    {
        ASSERT(ranges::bidirectional_range<R>);
        ASSERT(step < 0);
        ASSERT(start >= 0);
        ASSERT(stop >= 0);
        if (stop > start) { stop = start; }
        return get_reversed_range(
            rng |
            ranges::views::drop(stop + 1) |
            ranges::views::take(start - stop)
        ) | ranges::views::stride(-step);
    }

    template <typename R, typename = util::disable_lvalues_t<R>>
    [[nodiscard]] static auto pos_index_no_stop_neg_step(R&& rng, Int start, Int step)
    {
        ASSERT(ranges::bidirectional_range<R>);
        ASSERT(step < 0);
        ASSERT(start >= 0);
        return get_reversed_range(
            rng | ranges::views::take(start + 1)
        ) | ranges::views::stride(-step);
    }

    std::optional<Int> start_;
    std::optional<Int> stop_;
    std::optional<Int> step_;
};

struct FilterCreatorVisitor
{
    template <typename T>
    [[nodiscard]] FilterPtr operator()(const T& spec) const
    {
        return std::make_unique<FilterImpl<T>>(spec);
    }
};

} // namespace

FilterPtr Filter::create(const ast::FilterSpec& spec)
{
    return std::visit(FilterCreatorVisitor{}, spec);
}

} // namespace sq::results
