/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_parser_FilterSpec_h_
#define SQ_INCLUDE_GUARD_parser_FilterSpec_h_

#include "common_types/Primitive.h"
#include "util/typeutil.h"

#include <compare>
#include <gsl/gsl>
#include <iosfwd>
#include <optional>
#include <variant>

namespace sq::parser {

/**
 * Represents the lack of a filter specification for a field access.
 */
struct NoFilterSpec {
  SQ_ND auto operator<=>(const NoFilterSpec &) const = default;
};
std::ostream &operator<<(std::ostream &os, NoFilterSpec nlfs);

/**
 * Represents access of an indexed element in a list of results for a field
 * access.
 */
struct ElementAccessSpec {
  gsl::index index_;
  SQ_ND auto operator<=>(const ElementAccessSpec &) const = default;
};
std::ostream &operator<<(std::ostream &os, ElementAccessSpec leas);

/**
 * Represents a Python-style slice of a list of results for a field access.
 */
struct SliceSpec {
  std::optional<gsl::index> start_;
  std::optional<gsl::index> stop_;
  std::optional<gsl::index> step_;
  SQ_ND auto operator<=>(const SliceSpec &) const = default;
};
std::ostream &operator<<(std::ostream &os, SliceSpec lss);

enum class ComparisonOperator {
  GreaterThanOrEqualTo,
  GreaterThan,
  LessThanOrEqualTo,
  LessThan,
  Equals
};

/**
 * Represents a comparison to determine whether to keep a field or not.
 */
struct ComparisonSpec {
  std::string member_;
  ComparisonOperator op_;
  Primitive value_;
  SQ_ND auto operator<=>(const ComparisonSpec &) const = default;
};

std::ostream &operator<<(std::ostream &os, const ComparisonOperator &op);
std::ostream &operator<<(std::ostream &os, const ComparisonSpec &cs);

using FilterSpec =
    std::variant<NoFilterSpec, ElementAccessSpec, SliceSpec, ComparisonSpec>;

} // namespace sq::parser

#endif // SQ_INCLUDE_GUARD_parser_FilterSpec_h_
