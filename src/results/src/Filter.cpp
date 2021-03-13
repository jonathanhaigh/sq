/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/Filter.h"

#include "common_types/FieldCallParams.h"
#include "common_types/errors.h"
#include "util/ASSERT.h"
#include "util/SharedRange.h"
#include "util/typeutil.h"

#include <fmt/format.h>
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

namespace sq::results {

namespace {

SQ_ND auto slurp_range_into_vector(ranges::cpp20::range auto &&rng) {
  return util::SharedRange{std::make_shared<std::vector<FieldPtr>>(
      SQ_FWD(rng) | ranges::views::move | ranges::to<std::vector>())};
}

SQ_ND Result to_result(ranges::cpp20::range auto &&rng) {
  return FieldRange<ranges::get_categories<decltype(rng)>()>{SQ_FWD(rng)};
}

template <util::Alternative<parser::FilterSpec> Spec> struct FilterImpl;

template <> struct FilterImpl<parser::NoFilterSpec> : Filter {
  explicit FilterImpl(SQ_MU const parser::NoFilterSpec &spec) {}

  SQ_ND Result operator()(Result &&result) const override {
    return std::move(result);
  }
};

template <> struct FilterImpl<parser::ElementAccessSpec> : Filter {
  explicit FilterImpl(parser::ElementAccessSpec spec) : index_{spec.index_} {}

  SQ_ND Result operator()(Result &&result) const override {
    return std::visit(*this, std::move(result));
  }

  SQ_ND Result operator()(SQ_MU const FieldPtr &fp) const {
    throw NotAnArrayError{"Cannot apply array filter to non-array field"};
  }

  SQ_ND Result operator()(ranges::cpp20::view auto &&rng) const {
    if (index_ >= 0) {
      return nonnegative_index_access(SQ_FWD(rng), index_);
    }
    if constexpr (!util::SlowSizedRange<decltype(rng)>) {
      return (*this)(to_result(slurp_range_into_vector(rng)));
    } else {
      const auto size = util::to_index(ranges::distance(rng));
      return negative_index_access(SQ_FWD(rng), index_, size);
    }
  }

private:
  SQ_ND Result nonnegative_index_access(ranges::cpp20::view auto &&rng,
                                        gsl::index index) const {
    Expects(index >= 0);
    auto it = ranges::begin(rng);
    auto end = ranges::end(rng);
    ranges::advance(it, index, end);
    if (it == end) {
      auto message =
          fmt::format("array element access (\"[{}]\"): out of range", index_);
      if constexpr (util::SlowSizedRange<decltype(rng)>) {
        message += fmt::format("(size={})", ranges::distance(rng));
      }
      // clang-tidy bug: https://reviews.llvm.org/D72333
      // NOLINTNEXTLINE(readability-misleading-indentation)
      throw OutOfRangeError{message};
    }
    return std::move(*it);
  }

  SQ_ND Result negative_index_access(ranges::cpp20::view auto &&rng,
                                     gsl::index index, gsl::index size) const {
    Expects(index < 0);
    auto nonneg_index = size + index;
    if (nonneg_index < 0) {
      throw OutOfRangeError{
          fmt::format("array element access (\"[{}]\"): out of range (size={})",
                      index_, size)};
    }
    return nonnegative_index_access(SQ_FWD(rng), nonneg_index);
  }

  gsl::index index_;
};

template <> struct FilterImpl<parser::SliceSpec> : Filter {
  explicit FilterImpl(parser::SliceSpec spec)
      : start_{spec.start_}, stop_{spec.stop_}, step_{spec.step_} {}

  SQ_ND Result operator()(Result &&result) const override {
    return std::visit(*this, std::move(result));
  }

  SQ_ND Result operator()(SQ_MU const FieldPtr &fp) const {
    throw NotAnArrayError{"Cannot apply array filter to non-array field"};
  }

  SQ_ND Result operator()(ranges::cpp20::view auto &&rng) const {
    auto step = step_.value_or(1);
    return step > 0 ? pos_step(SQ_FWD(rng), step) : neg_step(SQ_FWD(rng), step);
  }

private:
  SQ_ND Result pos_step(ranges::cpp20::view auto &&rng, gsl::index step) const {
    auto start = start_.value_or(0);
    auto stop = stop_.value_or(0);
    if (start < 0 || (stop_ && stop < 0)) {
      if constexpr (!util::SlowSizedRange<decltype(rng)>) {
        return (*this)(to_result(slurp_range_into_vector(rng)));
      } else {
        return stop_ ? to_result(
                           mixed_index_pos_step(SQ_FWD(rng), start, stop, step))
                     : to_result(neg_index_no_stop_pos_step(SQ_FWD(rng), start,
                                                            step));
      }
    }
    return stop_ ? to_result(pos_index_pos_step(SQ_FWD(rng), start, stop, step))
                 : to_result(
                       pos_index_no_stop_pos_step(SQ_FWD(rng), start, step));
  }

  SQ_ND Result neg_step(ranges::cpp20::view auto &&rng, gsl::index step) const {
    if constexpr (!ranges::cpp20::bidirectional_range<decltype(rng)>) {
      return (*this)(to_result(slurp_range_into_vector(rng)));
    } else {
      auto start = start_.value_or(-1);
      auto stop = stop_.value_or(-1);
      if (start >= 0 && stop >= 0) {
        return to_result(pos_index_neg_step(SQ_FWD(rng), start, stop, step));
      }
      if (start >= 0) {
        return stop_ ? to_result(
                           mixed_index_neg_step(SQ_FWD(rng), start, stop, step))
                     : to_result(pos_index_no_stop_neg_step(SQ_FWD(rng), start,
                                                            step));
      }
      if (stop >= 0) {
        return to_result(mixed_index_neg_step(SQ_FWD(rng), start, stop, step));
      }
      return stop_
                 ? to_result(neg_index_neg_step(SQ_FWD(rng), start, stop, step))
                 : to_result(
                       neg_index_no_stop_neg_step(SQ_FWD(rng), start, step));
    }
  }

  SQ_ND static auto pos_index_pos_step(ranges::cpp20::view auto &&rng,
                                       gsl::index start, gsl::index stop,
                                       gsl::index step) {
    Expects(step > 0);
    Expects(start >= 0);
    Expects(stop >= 0);
    if (stop < start) {
      stop = start;
    }
    return SQ_FWD(rng) | ranges::views::drop(start) |
           ranges::views::take(stop - start) | ranges::views::stride(step);
  }

  SQ_ND static auto pos_index_no_stop_pos_step(ranges::cpp20::view auto &&rng,
                                               gsl::index start,
                                               gsl::index step) {
    Expects(start >= 0);
    Expects(step > 0);
    return SQ_FWD(rng) | ranges::views::drop(start) |
           ranges::views::stride(step);
  }

  SQ_ND static auto mixed_index_pos_step(util::SlowSizedRange auto &&rng,
                                         gsl::index start, gsl::index stop,
                                         gsl::index step) {
    Expects(step > 0);
    Expects(start < 0 || stop < 0);
    auto size = util::to_index(ranges::distance(rng));

    if (start < 0) {
      start += size;
    };
    if (start < 0) {
      start = 0;
    }
    if (stop < 0) {
      stop += size;
    }
    if (stop < 0) {
      stop = start;
    }
    return pos_index_pos_step(SQ_FWD(rng), start, stop, step);
  }

  SQ_ND static auto neg_index_no_stop_pos_step(util::SlowSizedRange auto &&rng,
                                               gsl::index start,
                                               gsl::index step) {
    Expects(step > 0);
    Expects(start < 0);
    start += util::to_index(ranges::distance(rng));
    if (start < 0) {
      start = 0;
    }
    return pos_index_no_stop_pos_step(SQ_FWD(rng), start, step);
  }

  SQ_ND static auto
  neg_index_neg_step(ranges::cpp20::bidirectional_range auto &&rng,
                     gsl::index start, gsl::index stop, gsl::index step) {
    Expects(step < 0);
    Expects(start < 0);
    Expects(stop < 0);
    start = -start - 1;
    stop = -stop - 1;
    step *= -1;
    return pos_index_pos_step(SQ_FWD(rng) | ranges::views::reverse, start, stop,
                              step);
  }

  SQ_ND static auto
  neg_index_no_stop_neg_step(ranges::cpp20::bidirectional_range auto &&rng,
                             gsl::index start, gsl::index step) {
    Expects(step < 0);
    Expects(start < 0);
    start = -start - 1;
    step *= -1;
    return pos_index_no_stop_pos_step(SQ_FWD(rng) | ranges::views::reverse,
                                      start, step);
  }

  SQ_ND static auto
  mixed_index_neg_step(ranges::cpp20::bidirectional_range auto &&rng,
                       gsl::index start, gsl::index stop, gsl::index step) {
    Expects(step < 0);
    Expects(start >= 0 || stop >= 0);
    auto size = util::to_index(ranges::distance(rng));

    if (start >= 0) {
      start -= size;
    };
    if (start >= 0) {
      start = -1;
    }
    if (stop >= 0) {
      stop -= size;
    }
    if (stop >= 0) {
      stop = start;
    }
    return neg_index_neg_step(SQ_FWD(rng), start, stop, step);
  }

  SQ_ND static auto
  pos_index_neg_step(ranges::cpp20::bidirectional_range auto &&rng,
                     gsl::index start, gsl::index stop, gsl::index step) {
    Expects(step < 0);
    Expects(start >= 0);
    Expects(stop >= 0);
    if (stop > start) {
      stop = start;
    }

    return SQ_FWD(rng) | ranges::views::drop(stop + 1) |
           ranges::views::take(start - stop) | ranges::views::reverse |
           ranges::views::stride(-step);
  }

  SQ_ND static auto
  pos_index_no_stop_neg_step(ranges::cpp20::bidirectional_range auto &&rng,
                             gsl::index start, gsl::index step) {
    Expects(step < 0);
    Expects(start >= 0);

    return SQ_FWD(rng) | ranges::views::take(start + 1) |
           ranges::views::reverse | ranges::views::stride(-step);
  }

  std::optional<gsl::index> start_;
  std::optional<gsl::index> stop_;
  std::optional<gsl::index> step_;
};

template <> struct FilterImpl<parser::ComparisonSpec> : Filter {
  using CompareFn = std::function<bool(const FieldPtr &)>;

  explicit FilterImpl(SQ_MU const parser::ComparisonSpec &spec) : spec_{spec} {}

  SQ_ND Result operator()(Result &&result) const override {
    return std::visit(*this, std::move(result));
  }

  SQ_ND Result operator()(SQ_MU const FieldPtr &field) const {
    throw NotAnArrayError{"Cannot apply array filter to non-array field"};
  }

  SQ_ND Result operator()(ranges::cpp20::view auto &&rng) const {
    return to_result(SQ_FWD(rng) |
                     // If rng is allocating and constructing a new Field on
                     // every dereference, make sure we only do that once by
                     // caching the FieldPtr.
                     ranges::views::cache1 |
                     ranges::views::filter([&](const FieldPtr &field) {
                       return compare(field);
                     }));
  }

private:
  SQ_ND FieldPtr get_member(const FieldPtr &field) const {
    if (spec_.member_.empty()) {
      return field;
    }
    auto res = field->get(spec_.member_, FieldCallParams{});
    if (std::holds_alternative<FieldPtr>(res)) {
      return std::get<FieldPtr>(std::move(res));
    }
    throw NotAScalarError{
        fmt::format("Cannot filter list by comparison of member \"{}\""
                    " with value {} using operator \"{}\":"
                    " \"{}\" is not a scalar field",
                    spec_.member_, primitive_to_str(spec_.value_), spec_.op_,
                    spec_.member_)};
  }

  SQ_ND bool compare(const FieldPtr &field) const {
    auto member_value = get_member(field)->to_primitive();
    switch (spec_.op_) {
    case parser::ComparisonOperator::GreaterThanOrEqualTo:
      // https://bugs.llvm.org/show_bug.cgi?id=46235
      // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
      return member_value >= spec_.value_;
    case parser::ComparisonOperator::GreaterThan:
      // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
      return member_value > spec_.value_;
    case parser::ComparisonOperator::LessThanOrEqualTo:
      // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
      return member_value <= spec_.value_;
    case parser::ComparisonOperator::LessThan:
      // NOLINTNEXTLINE(hicpp-use-nullptr,modernize-use-nullptr)
      return member_value < spec_.value_;
    case parser::ComparisonOperator::Equals:
      return member_value == spec_.value_;
    }
    ASSERT(false);
    throw InternalError{"Invalid comparison operator"};
  }

  parser::ComparisonSpec spec_;
};

struct FilterCreatorVisitor {
  template <util::Alternative<parser::FilterSpec> T>
  SQ_ND FilterPtr operator()(const T &spec) const {
    return std::make_unique<FilterImpl<T>>(spec);
  }
};

} // namespace

FilterPtr Filter::create(const parser::FilterSpec &spec) {
  return std::visit(FilterCreatorVisitor{}, spec);
}

} // namespace sq::results
