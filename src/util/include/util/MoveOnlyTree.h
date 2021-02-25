/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_util_MoveOnlyTree_h_
#define SQ_INCLUDE_GUARD_util_MoveOnlyTree_h_

#include <iostream>
#include <vector>

namespace sq::util {

template <typename T>
class MoveOnlyTree
{
public:
    using Children = std::vector<MoveOnlyTree<T>>;

    MoveOnlyTree() = delete;
    MoveOnlyTree(const MoveOnlyTree&) = delete;
    MoveOnlyTree& operator=(const MoveOnlyTree&) = delete;

    MoveOnlyTree(MoveOnlyTree&&) noexcept = default;
    MoveOnlyTree& operator=(MoveOnlyTree&&) noexcept = default;
    ~MoveOnlyTree() noexcept = default;

    template <typename... Args>
    explicit MoveOnlyTree(Args&&... args)
        : data_{std::forward<Args>(args)...}
    { }

    [[nodiscard]] const T& data() const noexcept { return data_; }
    [[nodiscard]] T& data() noexcept { return data_; }

    [[nodiscard]] const Children& children() const noexcept { return children_; }
    [[nodiscard]] Children& children() noexcept { return children_; }

    [[nodiscard]] friend bool operator==(const MoveOnlyTree& lhs, const MoveOnlyTree& rhs) {
        return lhs.children_ == rhs.children_ && lhs.data_ == rhs.data_;
    }
    [[nodiscard]] friend bool operator!=(const MoveOnlyTree& lhs, const MoveOnlyTree& rhs) {
        return !(lhs == rhs);
    }
 
private:
    Children children_;
    T data_;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const MoveOnlyTree<T>& tree)
{
    os << tree.data();
    if (!tree.children().empty())
    {
        os << " { ";
        for (const auto& child : tree.children())
        {
            os << child << " ";
        }
        os << "} ";
    }
    return os;
}

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_MoveOnlyTree_h_
