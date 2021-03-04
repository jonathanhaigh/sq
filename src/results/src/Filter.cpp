/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/Filter.h"

#include "common_types/InternalError.h"
#include "common_types/NotAnArrayError.h"
#include "common_types/OutOfRangeError.h"
#include "util/ASSERT.h"
#include "util/SharedRange.h"
#include "util/typeutil.h"

#include <functional>
#include <gsl/gsl>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_access.hpp>
#include <range/v3/view/cache1.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/stride.hpp>
#include <range/v3/view/take.hpp>
#include <sstream>

namespace sq::results {

namespace {

template <ranges::cpp20::range R>
[[nodiscard]] auto slurp_range_into_vector(R&& rng)
{
    return FieldRange<
        ranges::category::random_access | ranges::category::sized
    >{
        util::SharedRange{
            std::make_shared<std::vector<FieldPtr>>(
                std::forward<R>(rng) | ranges::views::move | ranges::to<std::vector>()
            )
        }
    };
}

template <ranges::cpp20::range R>
[[nodiscard]] Result to_result(R&& rng)
{
    return FieldRange<ranges::get_categories<R>()>{std::forward<R>(rng)};
}


template <util::Alternative<parser::FilterSpec> Spec>
struct FilterImpl;

template <>
struct FilterImpl<parser::NoFilterSpec>
    : Filter
{
    explicit FilterImpl([[maybe_unused]] const parser::NoFilterSpec& spec)
    { }

    [[nodiscard]] Result operator()(Result&& result) const override
    {
        return std::move(result);
    }
};

template <>
struct FilterImpl<parser::ElementAccessSpec>
    : Filter
{
    explicit FilterImpl(parser::ElementAccessSpec spec)
        : index_{spec.index_}
    { }

    [[nodiscard]] Result operator()(Result&& result) const override
    {
        return std::visit(*this, std::move(result));
    }

    [[nodiscard]] Result operator()([[maybe_unused]] const FieldPtr& fp) const
    {
        throw NotAnArrayError("Cannot apply array filter to non-array field");
    }

    template <ranges::cpp20::view R>
    [[nodiscard]] Result operator()(R&& rng) const
    {
        if (index_ >= 0)
        {
            return nonnegative_index_access(std::forward<R>(rng), index_);
        }
        if constexpr (!ranges::forward_range<R> && !ranges::sized_range<R>)
        {
            return (*this)(slurp_range_into_vector(rng));
        }
        else
        {
            const auto size = util::to_index(ranges::distance(rng));
            return negative_index_access(std::forward<R>(rng), index_, size);
        }
    }

private:

    template <ranges::cpp20::view R>
    [[nodiscard]] Result nonnegative_index_access(R&& rng, gsl::index index) const
    {
        Expects(index >= 0);
        auto it = ranges::begin(rng);
        auto end = ranges::end(rng);
        ranges::advance(it, index, end);
        if (it == end)
        {
            std::ostringstream ss;
            ss << "array element access (\"[" << index_ << "]\"): out of range";
            if constexpr (ranges::forward_range<R> || ranges::sized_range<R>)
            {
                ss << "(size=" << ranges::distance(rng) << ")";
            }
            // clang-tidy bug: https://reviews.llvm.org/D72333
            // NOLINTNEXTLINE(readability-misleading-indentation)
            throw OutOfRangeError(ss.str());
        }
        return std::move(*it);
    }

    template <ranges::cpp20::view R>
    [[nodiscard]] Result negative_index_access(R&& rng, gsl::index index, gsl::index size) const
    {
        Expects(index < 0);
        auto nonneg_index = size + index;
        if (nonneg_index < 0)
        {
            std::ostringstream ss;
            ss << "array element access (\"[" << index_ << "]\"): out of range (size=" << size << ")";
            throw OutOfRangeError(ss.str());
        }
        return nonnegative_index_access(std::forward<R>(rng), nonneg_index);
    }

    gsl::index index_;
};

template <>
struct FilterImpl<parser::SliceSpec>
    : Filter
{
    explicit FilterImpl(parser::SliceSpec spec)
        : start_{spec.start_}
        , stop_{spec.stop_}
        , step_{spec.step_}
    { }

    [[nodiscard]] Result operator()(Result&& result) const override
    {
        return std::visit(*this, std::move(result));
    }

    [[nodiscard]] Result operator()([[maybe_unused]] const FieldPtr& fp) const
    {
        throw NotAnArrayError("Cannot apply array filter to non-array field");
    }

    template <ranges::cpp20::view R>
    [[nodiscard]] Result operator()(R&& rng) const
    {
        auto step = step_.value_or(1);
        return step > 0?
            pos_step(std::forward<R>(rng), step) :
            neg_step(std::forward<R>(rng), step);
    }

private:
    template <ranges::cpp20::view R>
    [[nodiscard]] Result pos_step(R&& rng, gsl::index step) const
    {
        auto start = start_.value_or(0);
        auto stop = stop_.value_or(0);
        if (start < 0 || (stop_ && stop < 0))
        {
            if constexpr (!ranges::forward_range<R> && !ranges::sized_range<R>)
            {
                return (*this)(slurp_range_into_vector(rng));
            }
            else
            {
                return stop_?
                    to_result(mixed_index_pos_step(std::forward<R>(rng), start, stop, step)) :
                    to_result(neg_index_no_stop_pos_step(std::forward<R>(rng), start, step));
            }
        }
        return stop_?
            to_result(pos_index_pos_step(std::forward<R>(rng), start, stop, step)) :
            to_result(pos_index_no_stop_pos_step(std::forward<R>(rng), start, step));
    }

    template <ranges::cpp20::view R>
    [[nodiscard]] Result neg_step(R&& rng, gsl::index step) const
    {
        if constexpr (!ranges::bidirectional_range<R>)
        {
            return (*this)(slurp_range_into_vector(rng));
        }
        else
        {
            auto start = start_.value_or(-1);
            auto stop = stop_.value_or(-1);
            if (start >=0 && stop >=0)
            {
                return to_result(pos_index_neg_step(std::forward<R>(rng), start, stop, step));
            }
            if (start >= 0)
            {
                return stop_?
                    to_result(mixed_index_neg_step(std::forward<R>(rng), start, stop, step)) :
                    to_result(pos_index_no_stop_neg_step(std::forward<R>(rng), start, step));
            }
            if (stop >= 0)
            {
                return to_result(mixed_index_neg_step(std::forward<R>(rng), start, stop, step));
            }
            return stop_?
                to_result(neg_index_neg_step(std::forward<R>(rng), start, stop, step)) :
                to_result(neg_index_no_stop_neg_step(std::forward<R>(rng), start, step));
        }
    }

    template <ranges::cpp20::view R>
    [[nodiscard]] static auto pos_index_pos_step(R&& rng, gsl::index start, gsl::index stop, gsl::index step)
    {
        Expects(step > 0);
        Expects(start >= 0);
        Expects(stop >= 0);
        if (stop < start) { stop = start; }
        return std::forward<R>(rng) |
               ranges::views::drop(start) |
               ranges::views::take(stop - start) |
               ranges::views::stride(step);
    }

    template <ranges::cpp20::view R>
    [[nodiscard]] static auto pos_index_no_stop_pos_step(R&& rng, gsl::index start, gsl::index step)
    {
        Expects(start >= 0);
        Expects(step > 0);
        return std::forward<R>(rng) |
               ranges::views::drop(start) |
               ranges::views::stride(step);
    }

    template <ranges::cpp20::view R>
        requires ranges::forward_range<R> || ranges::sized_range<R>
    [[nodiscard]] static auto mixed_index_pos_step(R&& rng, gsl::index start, gsl::index stop, gsl::index step)
    {
        Expects(step > 0);
        Expects(start < 0 || stop < 0);
        auto size = util::to_index(ranges::distance(rng));

        if (start < 0) { start += size; };
        if (start < 0){ start = 0; }
        if (stop < 0) { stop += size; }
        if (stop < 0) { stop = start; }
        return pos_index_pos_step(std::forward<R>(rng), start, stop, step);
    }

    template <ranges::cpp20::view R>
        requires ranges::forward_range<R> || ranges::sized_range<R>
    [[nodiscard]] static auto neg_index_no_stop_pos_step(R&& rng, gsl::index start, gsl::index step)
    {
        Expects(step > 0);
        Expects(start < 0);
        start += util::to_index(ranges::distance(rng));
        if (start < 0) { start = 0; }
        return pos_index_no_stop_pos_step(std::forward<R>(rng), start, step);
    }

    template <ranges::cpp20::view R>
        requires ranges::bidirectional_range<R>
    [[nodiscard]] static auto neg_index_neg_step(R&& rng, gsl::index start, gsl::index stop, gsl::index step)
    {
        Expects(step < 0);
        Expects(start < 0);
        Expects(stop < 0);
        start = -start - 1;
        stop = -stop - 1;
        step *= -1;
        return pos_index_pos_step(
            std::forward<R>(rng) | ranges::views::reverse,
            start, stop, step
        );
    }

    template <ranges::cpp20::view R>
        requires ranges::bidirectional_range<R>
    [[nodiscard]] static auto neg_index_no_stop_neg_step(R&& rng, gsl::index start, gsl::index step)
    {
        Expects(step < 0);
        Expects(start < 0);
        start = -start -1;
        step *= -1;
        return pos_index_no_stop_pos_step(
           std::forward<R>(rng) | ranges::views::reverse,
           start, step
       );
    }

    template <ranges::cpp20::view R>
        requires ranges::bidirectional_range<R>
    [[nodiscard]] static auto mixed_index_neg_step(R&& rng, gsl::index start, gsl::index stop, gsl::index step)
    {
        Expects(step < 0);
        Expects(start >= 0 || stop >= 0);
        auto size = util::to_index(ranges::distance(rng));

        if (start >= 0) { start -= size; };
        if (start >= 0){ start = -1; }
        if (stop >= 0) { stop -= size; }
        if (stop >= 0) { stop = start; }
        return neg_index_neg_step(std::forward<R>(rng), start, stop, step);
    }

    template <ranges::cpp20::view R>
        requires ranges::bidirectional_range<R>
    [[nodiscard]] static auto pos_index_neg_step(R&& rng, gsl::index start, gsl::index stop, gsl::index step)
    {
        Expects(step < 0);
        Expects(start >= 0);
        Expects(stop >= 0);
        if (stop > start) { stop = start; }

        return std::forward<R>(rng) |
            ranges::views::drop(stop + 1) |
            ranges::views::take(start - stop) |
            ranges::views::reverse |
            ranges::views::stride(-step);
    }

    template <ranges::cpp20::view R>
        requires ranges::bidirectional_range<R>
    [[nodiscard]] static auto pos_index_no_stop_neg_step(R&& rng, gsl::index start, gsl::index step)
    {
        Expects(step < 0);
        Expects(start >= 0);

        return std::forward<R>(rng) |
            ranges::views::take(start + 1) |
            ranges::views::reverse |
            ranges::views::stride(-step);
    }

    std::optional<gsl::index> start_;
    std::optional<gsl::index> stop_;
    std::optional<gsl::index> step_;
};

template <>
struct FilterImpl<parser::ComparisonSpec>
    : Filter
{
    using CompareFn = std::function<bool(const FieldPtr&)>;

    static CompareFn create_compare_fn(const parser::ComparisonSpec& spec)
    {
        switch (spec.op_)
        {
            case parser::ComparisonOperator::GreaterThanOrEqualTo:
                return [value=spec.value_](const FieldPtr& field) {
                    // https://bugs.llvm.org/show_bug.cgi?id=46235
                    // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
                    return field->to_primitive() >= value;
                };
            case parser::ComparisonOperator::GreaterThan:
                return [value=spec.value_](const FieldPtr& field) {
                    // https://bugs.llvm.org/show_bug.cgi?id=46235
                    // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
                    return field->to_primitive() > value;
                };
            case parser::ComparisonOperator::LessThanOrEqualTo:
                return [value=spec.value_](const FieldPtr& field) {
                    // https://bugs.llvm.org/show_bug.cgi?id=46235
                    // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
                    return field->to_primitive() <= value;
                };
            case parser::ComparisonOperator::LessThan:
                return [value=spec.value_](const FieldPtr& field) {
                    // https://bugs.llvm.org/show_bug.cgi?id=46235
                    // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
                    return field->to_primitive() < value;
                };
            case parser::ComparisonOperator::Equals:
                return [value=spec.value_](const FieldPtr& field) {
                    // https://bugs.llvm.org/show_bug.cgi?id=46235
                    // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
                    return field->to_primitive() == value;
                };
        }
        ASSERT(false);
        throw InternalError{"Invalid comparison operator"};
    }

    explicit FilterImpl([[maybe_unused]] const parser::ComparisonSpec& spec)
        : compare_{create_compare_fn(spec)}
    { }

    [[nodiscard]] Result operator()(Result&& result) const override
    {
        return std::visit(*this, std::move(result));
    }

    [[nodiscard]] Result operator()([[maybe_unused]] const FieldPtr& field) const
    {
        throw NotAnArrayError("Cannot apply array filter to non-array field");
    }

    template <ranges::cpp20::view R>
    [[nodiscard]] Result operator()(R&& rng) const
    {
        return to_result(
            std::forward<R>(rng) |
            // If rng is allocating and constructing a new Field on every
            // dereference, make sure we only do that once by caching the
            // FieldPtr.
            ranges::views::cache1 |
            ranges::views::filter(compare_)
        );
    }

private:
    CompareFn compare_;
};

struct FilterCreatorVisitor
{
    template <util::Alternative<parser::FilterSpec> T>
    [[nodiscard]] FilterPtr operator()(const T& spec) const
    {
        return std::make_unique<FilterImpl<T>>(spec);
    }
};

} // namespace

FilterPtr Filter::create(const parser::FilterSpec& spec)
{
    return std::visit(FilterCreatorVisitor{}, spec);
}

} // namespace sq::results
