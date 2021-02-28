/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_parser_FilterSpec_h_
#define SQ_INCLUDE_GUARD_parser_FilterSpec_h_

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

using FilterSpec = std::variant<
    NoFilterSpec,
    ElementAccessSpec,
    SliceSpec
>;

} // namespace sq::parser

#endif // SQ_INCLUDE_GUARD_parser_FilterSpec_h_
