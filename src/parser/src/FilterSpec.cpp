/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "parser/FilterSpec.h"

#include "common_types/Primitive.h"
#include "util/ASSERT.h"
#include "util/strutil.h"
#include "util/typeutil.h"

#include <iostream>

namespace sq::parser {

namespace {

const char *comparison_operator_to_str(ComparisonOperator op) {
  switch (op) {
  case ComparisonOperator::GreaterThanOrEqualTo:
    return ">=";
  case ComparisonOperator::GreaterThan:
    return ">";
  case ComparisonOperator::LessThanOrEqualTo:
    return "<=";
  case ComparisonOperator::LessThan:
    return "<";
  case ComparisonOperator::Equals:
    return "=";
  }
  ASSERT(false);
  return "Unknown ComparisonOperator";
}

} // namespace

std::ostream &operator<<(std::ostream &os, SQ_MU NoFilterSpec nlfs) {
  return os;
}

bool operator==(SQ_MU NoFilterSpec lhs, SQ_MU NoFilterSpec rhs) { return true; }

bool operator!=(SQ_MU NoFilterSpec lhs, SQ_MU NoFilterSpec rhs) {
  return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, ElementAccessSpec leas) {
  os << leas.index_;
  return os;
}

bool operator==(ElementAccessSpec lhs, ElementAccessSpec rhs) {
  return lhs.index_ == rhs.index_;
}

bool operator!=(ElementAccessSpec lhs, ElementAccessSpec rhs) {
  return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, SliceSpec lss) {
  os << util::optional_to_str(lss.start_) << ":"
     << util::optional_to_str(lss.stop_) << ":"
     << util::optional_to_str(lss.step_);
  return os;
}

bool operator==(const SliceSpec &lhs, const SliceSpec &rhs) {
  return lhs.start_ == rhs.start_ && lhs.stop_ == rhs.stop_ &&
         lhs.step_ == rhs.step_;
}

bool operator!=(const SliceSpec &lhs, const SliceSpec &rhs) {
  return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, const ComparisonOperator &op) {
  os << comparison_operator_to_str(op);
  return os;
}

std::ostream &operator<<(std::ostream &os, const ComparisonSpec &cs) {
  os << cs.member_ << cs.op_ << primitive_to_str(cs.value_);
  return os;
}

bool operator==(const ComparisonSpec &lhs, const ComparisonSpec &rhs) {
  return lhs.member_ == rhs.member_ && lhs.op_ == rhs.op_ &&
         lhs.value_ == rhs.value_;
}

bool operator!=(const ComparisonSpec &lhs, const ComparisonSpec &rhs) {
  return !(lhs == rhs);
}

} // namespace sq::parser
