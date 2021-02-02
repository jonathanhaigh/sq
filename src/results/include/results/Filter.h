#ifndef SQ_INCLUDE_GUARD_results_Filter_h_
#define SQ_INCLUDE_GUARD_results_Filter_h_

#include "ast/FilterSpec.h"
#include "field_types/Field.h"
#include "results/ResultView.h"

#include <functional>
#include <memory>

namespace sq::results {

struct Filter;
using FilterPtr = std::unique_ptr<Filter>;

struct Filter
{
    /**
     * Create a Filter for the given spec.
     */
    static FilterPtr create(const ast::FilterSpec& spec);

    /**
     * Transform a Result so that it satisfies the requirements of this filter.
     *
     * I.e. if this filter requires a category of range that result can't
     * satisfy, transform it into a suitable range.
     *
     * E.g. FieldRange<ranges::category::input> can't handle element access
     * with negative indeces so this function might slurp a
     * FieldRange<ranges::category::input> into a FieldVector and return that
     * vector.
     */
    virtual Result transform_result_for_requirements(Result&& result) const = 0;

    /**
     * Apply this filter to a ResultView.
     */
    virtual ResultView view(ResultView&& result) const = 0;

    virtual ~Filter() = default;

protected:
    Filter() = default;

    Filter(const Filter&) = delete;
    Filter(Filter&&) = delete;
    Filter& operator=(const Filter&) = delete;
    Filter& operator=(Filter&&) = delete;
};


} // namespace sq::results

#endif // SQ_INCLUDE_GUARD_results_Filter_h_
