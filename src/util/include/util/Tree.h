#ifndef SQ_INCLUDE_GUARD_util_Tree_h_
#define SQ_INCLUDE_GUARD_util_Tree_h_

#include <iterator>
#include <vector>

#include <iostream>

namespace sq::util {

template <typename T>
class Tree
{
public:
    class Children {
    private:
        using Vector = std::vector<Tree<T>>;
    public:
        using iterator = typename Vector::iterator;
        using const_iterator = typename Vector::const_iterator;
        using reference = typename Vector::reference;
        using const_reference = typename Vector::const_reference;
        using size_type = typename Vector::size_type;

        Children(const Children&) = delete;
        Children& operator=(const Children&) = delete;

        Children() = default;
        Children(Children&&) = default;
        Children& operator=(Children&&) = default;
        ~Children() noexcept = default;
    
        iterator begin() noexcept { return v_.begin(); }
        const_iterator begin() const noexcept { return v_.cbegin(); }
        const_iterator cbegin() const noexcept { return v_.cbegin(); }

        iterator end() noexcept { return v_.end(); }
        const_iterator end() const noexcept { return v_.cend(); }
        const_iterator cend() const noexcept { return v_.cend(); }

        size_type size() const noexcept { return v_.size(); }
        bool empty() const noexcept { return v_.empty(); }

        template <typename... Args>
        reference emplace_back(Args&&... args) {
            return v_.emplace_back(std::forward<Args>(args)...);
        }

        reference front() { return v_.front(); }
        const_reference front() const { return v_.front(); }
        
        reference back() { return v_.back(); }
        const_reference back() const { return v_.back(); }
    private:
        Vector v_;
    };

    Tree() = delete;
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    Tree(Tree&&) = default;
    Tree& operator=(Tree&&) = default;
    ~Tree() noexcept = default;

    template <typename... Args>
    explicit Tree(Args&&... args)
        : data_{std::forward<Args>(args)...}
    { }

    const T& data() const noexcept { return data_; }
    T& data() noexcept { return data_; }

    const Children& children() const noexcept { return children_; }
    Children& children() noexcept { return children_; }
 
private:
    Children children_;
    T data_;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Tree<T>& tree)
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

#endif // SQ_INCLUDE_GUARD_util_Tree_h_
