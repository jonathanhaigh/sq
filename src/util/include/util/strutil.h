#ifndef SQ_INCLUDE_GUARD_util_strutil_h_
#define SQ_INCLUDE_GUARD_util_strutil_h_

namespace sq::util {

template<typename It>
struct ContainerJoiner
{
    enum class Quoted {
        Yes,
        No,
    };

    ContainerJoiner(It begin, It end, std::string_view delim, Quoted quoted=Quoted::No)
        : begin_{begin}
        , end_{end}
        , delim_{delim}
        , quoted_{quoted}
    { }

    void write_item(std::ostream& os, It it) const
    {
        if (it != begin_)
        {
            os << delim_;
        }

        if (quoted_ == Quoted::Yes)
        {
            os << std::quoted(*it);
        }
        else
        {
            os << (*it);
        }
    }

    std::ostream& write_range(std::ostream& os) const
    {
        for (auto it = begin_; it != end_; ++it)
        {
            write_item(os, it);
        }
        return os;
    }

    It begin_;
    It end_;
    std::string_view delim_;
    Quoted quoted_;
};

template <typename It>
std::ostream& operator<<(std::ostream& os, const ContainerJoiner<It>& cj)
{
    return cj.write_range(os);
}

template <typename C>
ContainerJoiner<typename C::const_iterator> join(const C& c, std::string_view delim=", ")
{
    using CJ = ContainerJoiner<typename C::const_iterator>;
    return CJ(std::begin(c), std::end(c), delim, CJ::Quoted::No);
}

template <typename C>
ContainerJoiner<typename C::const_iterator> quoted_join(const C& c, std::string_view delim=", ")
{
    using CJ = ContainerJoiner<typename C::const_iterator>;
    return CJ(std::begin(c), std::end(c), delim, CJ::Quoted::Yes);
}

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_strutil_h_
