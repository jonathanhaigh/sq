#include "ast/ListFilterSpec.h"

#include "util/strutil.h"

namespace sq::ast {

std::ostream& operator<<(std::ostream& os, ListElementAccessSpec leas)
{
    os << leas.index_;
    return os;
}

std::ostream& operator<<(std::ostream& os, ListSliceSpec lss)
{
    os << util::optional_to_str(lss.start_)
       << ":"
       << util::optional_to_str(lss.stop_)
       << ":"
       << util::optional_to_str(lss.step_);
    return os;
}

std::ostream& operator<<(std::ostream& os, [[maybe_unused]] NoListFilterSpec nlfs)
{
    return os;
}

} // namespace sq::ast
