/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "parser/FilterSpec.h"

#include "core/ASSERT.h"
#include "core/Primitive.h"
#include "core/strutil.h"
#include "core/typeutil.h"

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

std::ostream &operator<<(std::ostream &os, ElementAccessSpec leas) {
  os << leas.index_;
  return os;
}

std::ostream &operator<<(std::ostream &os, SliceSpec lss) {
  os << fmt::format("{}:{}:{}", lss.start_, lss.stop_, lss.step_);
  return os;
}

std::ostream &operator<<(std::ostream &os, const ComparisonOperator &op) {
  os << comparison_operator_to_str(op);
  return os;
}

std::ostream &operator<<(std::ostream &os, const ComparisonSpec &cs) {
  os << cs.member_ << cs.op_ << primitive_to_str(cs.value_);
  return os;
}

} // namespace sq::parser
