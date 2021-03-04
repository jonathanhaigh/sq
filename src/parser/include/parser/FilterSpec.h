/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_parser_FilterSpec_h_
#define SQ_INCLUDE_GUARD_parser_FilterSpec_h_

#include "common_types/Primitive.h"
#include "util/typeutil.h"

#include <gsl/gsl>
#include <iosfwd>
#include <optional>
#include <variant>

namespace sq::parser {

/**
 * Represents the lack of a filter specification for a field access.
 */
struct NoFilterSpec { };
std::ostream& operator<<(std::ostream& os, NoFilterSpec nlfs);
SQ_ND bool operator==(NoFilterSpec lhs, NoFilterSpec rhs);
SQ_ND bool operator!=(NoFilterSpec lhs, NoFilterSpec rhs);

/**
 * Represents access of an indexed element in a list of results for a field
 * access.
 */
struct ElementAccessSpec
{
    gsl::index index_;
};
std::ostream& operator<<(std::ostream& os, ElementAccessSpec leas);
SQ_ND bool operator==(ElementAccessSpec lhs, ElementAccessSpec rhs);
SQ_ND bool operator!=(ElementAccessSpec lhs, ElementAccessSpec rhs);

/**
 * Represents a Python-style slice of a list of results for a field access.
 */
struct SliceSpec
{
    std::optional<gsl::index> start_;
    std::optional<gsl::index> stop_;
    std::optional<gsl::index> step_;
};
std::ostream& operator<<(std::ostream& os, SliceSpec lss);
SQ_ND bool operator==(const SliceSpec& lhs, const SliceSpec& rhs);
SQ_ND bool operator!=(const SliceSpec& lhs, const SliceSpec& rhs);

enum class ComparisonOperator
{
    GreaterThanOrEqualTo,
    GreaterThan,
    LessThanOrEqualTo,
    LessThan,
    Equals
};

/**
 * Represents a comparison to determine whether to keep a field or not.
 */
struct ComparisonSpec
{
    ComparisonOperator op_;
    Primitive value_;
};

std::ostream& operator<<(std::ostream& os, const ComparisonOperator& op);
std::ostream& operator<<(std::ostream& os, const ComparisonSpec& cs);
SQ_ND bool operator==(const ComparisonSpec& lhs, const ComparisonSpec& rhs);
SQ_ND bool operator!=(const ComparisonSpec& lhs, const ComparisonSpec& rhs);

using FilterSpec = std::variant<
    NoFilterSpec,
    ElementAccessSpec,
    SliceSpec,
    ComparisonSpec
>;

} // namespace sq::parser

#endif // SQ_INCLUDE_GUARD_parser_FilterSpec_h_
