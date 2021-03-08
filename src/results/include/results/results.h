/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_results_generate_results_h_
#define SQ_INCLUDE_GUARD_results_generate_results_h_

#include "common_types/Field.h"
#include "common_types/Primitive.h"
#include "parser/Ast.h"
#include "util/typeutil.h"

#include <compare>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace sq::results {

/**
 * Tree representing the results of a query.
 *
 * Each node of the tree can represent either:
 * - a primitive type (a leaf node);
 * - a list of child results;
 * - a mapping from child field names to results for those children.
 */
class ResultTree {
public:
  using ObjData = std::vector<std::pair<std::string, ResultTree>>;
  using ArrayData = std::vector<ResultTree>;
  using Data = std::variant<ObjData, ArrayData, Primitive>;

  ResultTree() = delete;
  ResultTree(const ResultTree &) = delete;
  ResultTree &operator=(const ResultTree &) = delete;

  ResultTree(ResultTree &&other) = default;
  ResultTree &operator=(ResultTree &&other) = default;
  ~ResultTree() noexcept = default;

  ResultTree(const parser::Ast &ast, Result &&result);
  explicit ResultTree(Data &&data);

  ///@{
  /**
   * Get the data associated with this node.
   */
  SQ_ND const Data &data() const { return data_; }
  SQ_ND Data &data() { return data_; }
  ///@}

  SQ_ND bool operator==(const ResultTree &) const = default;

private:
  Data data_;
};

template <typename T>
concept ResultTreeDataAlternative = util::Alternative<T, ResultTree::Data>;

} // namespace sq::results

#endif // SQ_INCLUDE_GUARD_results_generate_results_h_
