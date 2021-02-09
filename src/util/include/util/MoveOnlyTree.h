#ifndef SQ_INCLUDE_GUARD_util_MoveOnlyTree_h_
#define SQ_INCLUDE_GUARD_util_MoveOnlyTree_h_

#include <iterator>
#include <vector>

#include <iostream>

namespace sq::util {

template <typename T>
class MoveOnlyTree
{
public:
    using Children = std::vector<MoveOnlyTree<T>>;

    MoveOnlyTree() = delete;
    MoveOnlyTree(const MoveOnlyTree&) = delete;
    MoveOnlyTree& operator=(const MoveOnlyTree&) = delete;

    MoveOnlyTree(MoveOnlyTree&&) = default;
    MoveOnlyTree& operator=(MoveOnlyTree&&) = default;
    ~MoveOnlyTree() noexcept = default;

    template <typename... Args>
    explicit MoveOnlyTree(Args&&... args)
        : data_{std::forward<Args>(args)...}
    { }

    const T& data() const noexcept { return data_; }
    T& data() noexcept { return data_; }

    const Children& children() const noexcept { return children_; }
    Children& children() noexcept { return children_; }

    friend bool operator==(const MoveOnlyTree& lhs, const MoveOnlyTree& rhs) {
        return lhs.children_ == rhs.children_ && lhs.data_ == rhs.data_;
    }
    friend bool operator!=(const MoveOnlyTree& lhs, const MoveOnlyTree& rhs) {
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
