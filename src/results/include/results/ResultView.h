#ifndef SQ_INCLUDE_GUARD_results_ResultView_h_
#define SQ_INCLUDE_GUARD_results_ResultView_h_

#include "common_types/Field.h"

#include <functional>
#include <range/v3/view/any_view.hpp>
#include <variant>

namespace sq::results {

using ResultView = std::variant<
    FieldPtr,
    FieldRange<ranges::category::input>,
    FieldRange<ranges::category::input | ranges::category::sized>,
    FieldRange<ranges::category::forward>,
    FieldRange<ranges::category::forward | ranges::category::sized>,
    FieldRange<ranges::category::bidirectional>,
    FieldRange<ranges::category::bidirectional | ranges::category::sized>,
    FieldRange<ranges::category::random_access>,
    FieldRange<ranges::category::random_access | ranges::category::sized>
>;

ResultView get_result_view(Result& result);

} // namespace sq::results

#endif // SQ_INCLUDE_GUARD_results_ResultView_h_
