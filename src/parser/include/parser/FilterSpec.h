/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_parser_FilterSpec_h_
#define SQ_INCLUDE_GUARD_parser_FilterSpec_h_

#include "common_types/Primitive.h"

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
[[nodiscard]] bool operator==(NoFilterSpec lhs, NoFilterSpec rhs);
[[nodiscard]] bool operator!=(NoFilterSpec lhs, NoFilterSpec rhs);

/**
 * Represents access of an indexed element in a list of results for a field
 * access.
 */
struct ElementAccessSpec
{
    gsl::index index_;
};
std::ostream& operator<<(std::ostream& os, ElementAccessSpec leas);
[[nodiscard]] bool operator==(ElementAccessSpec lhs, ElementAccessSpec rhs);
[[nodiscard]] bool operator!=(ElementAccessSpec lhs, ElementAccessSpec rhs);

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
[[nodiscard]] bool operator==(const SliceSpec& lhs, const SliceSpec& rhs);
[[nodiscard]] bool operator!=(const SliceSpec& lhs, const SliceSpec& rhs);

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
[[nodiscard]] bool operator==(const ComparisonSpec& lhs, const ComparisonSpec& rhs);
[[nodiscard]] bool operator!=(const ComparisonSpec& lhs, const ComparisonSpec& rhs);

using FilterSpec = std::variant<
    NoFilterSpec,
    ElementAccessSpec,
    SliceSpec,
    ComparisonSpec
>;

} // namespace sq::parser

#endif // SQ_INCLUDE_GUARD_parser_FilterSpec_h_
