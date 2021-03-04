/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_util_MoveOnlyTree_h_
#define SQ_INCLUDE_GUARD_util_MoveOnlyTree_h_

#include "util/typeutil.h"

#include <concepts>
#include <iostream>
#include <vector>

namespace sq::util {

/**
 * A move-only tree structure.
 *
 * @param T type of data to store in each node of the tree.
 */
template <typename T>
requires std::movable<T> && std::equality_comparable<T>
class MoveOnlyTree {
public:
  using Children = std::vector<MoveOnlyTree<T>>;

  MoveOnlyTree() = delete;
  MoveOnlyTree(const MoveOnlyTree &) = delete;
  MoveOnlyTree &operator=(const MoveOnlyTree &) = delete;

  MoveOnlyTree(MoveOnlyTree &&) noexcept = default;
  MoveOnlyTree &operator=(MoveOnlyTree &&) noexcept = default;
  ~MoveOnlyTree() noexcept = default;

  explicit MoveOnlyTree(auto &&...args) : data_{SQ_FWD(args)...} {}

  ///@{
  /**
   * Get the data associated with this node.
   */
  SQ_ND const T &data() const noexcept { return data_; }
  SQ_ND T &data() noexcept { return data_; }
  ///@}

  ///@{
  /**
   * Get the child nodes of this node.
   */
  SQ_ND const Children &children() const noexcept { return children_; }
  SQ_ND Children &children() noexcept { return children_; }
  ///@}

  SQ_ND friend bool operator==(const MoveOnlyTree &lhs,
                               const MoveOnlyTree &rhs) {
    return lhs.children_ == rhs.children_ && lhs.data_ == rhs.data_;
  }
  SQ_ND friend bool operator!=(const MoveOnlyTree &lhs,
                               const MoveOnlyTree &rhs) {
    return !(lhs == rhs);
  }

private:
  Children children_;
  T data_;
};

template <util::Printable T>
std::ostream &operator<<(std::ostream &os, const MoveOnlyTree<T> &tree) {
  os << tree.data();
  if (!tree.children().empty()) {
    os << " { ";
    for (const auto &child : tree.children()) {
      os << child << " ";
    }
    os << "} ";
  }
  return os;
}

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_MoveOnlyTree_h_
