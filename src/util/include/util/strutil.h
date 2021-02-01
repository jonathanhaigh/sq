#ifndef SQ_INCLUDE_GUARD_util_strutil_h_
#define SQ_INCLUDE_GUARD_util_strutil_h_

#include <iostream>
#include <sstream>
#include <string>
#include <variant>

namespace sq::util {

template<typename It>
class join
{
public:
    join(It begin, It end, std::string_view delim)
        : begin_{begin}
        , end_{end}
        , delim_{delim}
    { }

    join(It begin, It end)
        : begin_{begin}
        , end_{end}
    { }

    template <typename C>
    join(const C& c, std::string_view delim)
        : join(std::begin(c), std::end(c), delim)
    { }

    template <typename C>
    explicit join(const C& c)
        : join(std::begin(c), std::end(c))
    { }

private:
    void write_item(std::ostream& os, It it) const
    {
        if (it != begin_)
        {
            os << delim_;
        }

        os << (*it);
    }

    std::ostream& write_range(std::ostream& os) const
    {
        for (auto it = begin_; it != end_; ++it)
        {
            write_item(os, it);
        }
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const join<It>& cj)
    {
        return cj.write_range(os);
    }

    It begin_;
    It end_;
    std::string_view delim_ = ", ";
};

template <typename C>
join(const C& c, std::string_view delim) -> join<typename C::const_iterator>;

template <typename C>
join(const C& c) -> join<typename C::const_iterator>;


template <typename V>
std::string variant_to_str(V&& var)
{
    auto ss = std::ostringstream{};
    std::visit(
        [&ss](auto&& v) { ss << v; },
        std::forward<V>(var)
    );
    return ss.str();
}

template <typename T>
std::string optional_to_str(T&& var)
{
    if (var)
    {
        auto ss = std::ostringstream{};
        ss << *std::forward<T>(var);
        return ss.str();
    }
    return std::string{};
}

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_strutil_h_
