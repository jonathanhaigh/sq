/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_results_Filter_h_
#define SQ_INCLUDE_GUARD_results_Filter_h_

#include "common_types/Field.h"
#include "parser/FilterSpec.h"
#include "util/typeutil.h"

#include <gsl/gsl>
#include <memory>

namespace sq::results {

struct Filter;
using FilterPtr = std::unique_ptr<Filter>;

struct Filter {
  /**
   * Create a Filter for the given spec.
   */
  SQ_ND static FilterPtr create(const parser::FilterSpec &spec);

  /**
   * Apply this filter to a Result.
   */
  SQ_ND virtual Result operator()(Result &&result) const = 0;

  virtual ~Filter() = default;
  Filter() = default;
  Filter(const Filter &) = delete;
  Filter(Filter &&) = delete;
  Filter &operator=(const Filter &) = delete;
  Filter &operator=(Filter &&) = delete;
};

} // namespace sq::results

#endif // SQ_INCLUDE_GUARD_results_Filter_h_
