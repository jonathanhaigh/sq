#include "results/ResultView.h"

#include <range/v3/view/move.hpp>

namespace sq::results {

struct GetResultViewVisitor
{
    ResultView operator()(FieldPtr& fp)
    {
        return std::move(fp);
    }

    template <ranges::category Cat>
    ResultView operator()(FieldRange<Cat>& rng)
    {
        return std::move(rng);
    }

    ResultView operator()(FieldVector& vec)
    {
        return FieldRange<ranges::get_categories<FieldVector>()>{vec | ranges::views::move};
    }
};

ResultView get_result_view(Result& result)
{
    return std::visit(GetResultViewVisitor{}, result);
}

} // namespace sq::results
