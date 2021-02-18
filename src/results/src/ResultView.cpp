#include "results/ResultView.h"

#include <range/v3/view/move.hpp>

namespace sq::results {

namespace {

struct GetResultViewVisitor
{
    [[nodiscard]] ResultView operator()(FieldPtr& fp)
    {
        return std::move(fp);
    }

    template <ranges::category Cat>
    [[nodiscard]] ResultView operator()(FieldRange<Cat>& rng)
    {
        return std::move(rng);
    }

    [[nodiscard]] ResultView operator()(FieldVector& vec)
    {
        return FieldRange<ranges::get_categories<FieldVector>()>{vec | ranges::views::move};
    }
};

} // namespace

ResultView get_result_view(Result& result)
{
    return std::visit(GetResultViewVisitor{}, result);
}

} // namespace sq::results
