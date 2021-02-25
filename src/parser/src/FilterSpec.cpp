/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "parser/FilterSpec.h"

#include "util/strutil.h"

#include <iostream>

namespace sq::parser {

std::ostream& operator<<(std::ostream& os, [[maybe_unused]] NoFilterSpec nlfs)
{
    return os;
}

bool operator==([[maybe_unused]] NoFilterSpec lhs, [[maybe_unused]] NoFilterSpec rhs)
{
    return true;
}

bool operator!=([[maybe_unused]] NoFilterSpec lhs, [[maybe_unused]] NoFilterSpec rhs)
{
    return !(lhs == rhs);
}


std::ostream& operator<<(std::ostream& os, ElementAccessSpec leas)
{
    os << leas.index_;
    return os;
}

bool operator==(ElementAccessSpec lhs, ElementAccessSpec rhs)
{
    return lhs.index_ == rhs.index_;
}

bool operator!=(ElementAccessSpec lhs, ElementAccessSpec rhs)
{
    return !(lhs == rhs);
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

bool operator==(const SliceSpec& lhs, const SliceSpec& rhs)
{
    return lhs.start_ == rhs.start_ &&
           lhs.stop_ == rhs.stop_ &&
           lhs.step_ == rhs.step_;
}

bool operator!=(const SliceSpec& lhs, const SliceSpec& rhs)
{
    return !(lhs == rhs);
}

} // namespace sq::parser
