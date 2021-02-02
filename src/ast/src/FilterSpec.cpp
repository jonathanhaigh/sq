#include "ast/FilterSpec.h"

#include "util/strutil.h"

#include <iostream>

namespace sq::ast {

std::ostream& operator<<(std::ostream& os, ElementAccessSpec leas)
{
    os << leas.index_;
    return os;
}

std::ostream& operator<<(std::ostream& os, SliceSpec lss)
{
    os << util::optional_to_str(lss.start_)
       << ":"
       << util::optional_to_str(lss.stop_)
       << ":"
       << util::optional_to_str(lss.step_);
    return os;
}

std::ostream& operator<<(std::ostream& os, [[maybe_unused]] NoFilterSpec nlfs)
{
    return os;
}

} // namespace sq::ast
