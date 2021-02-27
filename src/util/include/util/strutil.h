/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_util_strutil_h_
#define SQ_INCLUDE_GUARD_util_strutil_h_

#include <gsl/gsl>
#include <iostream>
#include <memory>
#include <range/v3/range/access.hpp>
#include <sstream>
#include <string>
#include <utility>
#include <variant>

namespace sq::util {

template<typename R>
struct join
{
    explicit join(const R& rng)
        : rng_{std::addressof(rng)}
    { }

    join(const R& rng, std::string_view delim)
        : rng_{std::addressof(rng)}
        , delim_{delim}
    { }

    friend std::ostream& operator<<(std::ostream& os, const join<R>& j)
    {
        auto it = ranges::begin(*(j.rng_));
        const auto end = ranges::end(*(j.rng_));
        while (it != end)
        {
            os << (*it);
            ++it;
            if (it != end)
            {
                os << j.delim_;
            }
        }
        return os;
    }

private:
    gsl::not_null<const R*> rng_;
    std::string_view delim_ = ", ";
};


namespace detail {

struct VariantToStr
{
    template <typename V>
    [[nodiscard]] std::string operator()(V&& var) const
    {
        auto ss = std::ostringstream{};
        std::visit(
            [&ss](auto&& v) { ss << v; },
            std::forward<V>(var)
        );
        return ss.str();
    }
};

struct OptionalToStr
{
    template <typename T>
    [[nodiscard]] std::string operator()(T&& var) const
    {
        if (var)
        {
            auto ss = std::ostringstream{};
            ss << std::forward<T>(var).value();
            return ss.str();
        }
        return std::string{};
    }
};

} // namespace detail

inline constexpr auto variant_to_str = detail::VariantToStr{};
inline constexpr auto optional_to_str = detail::OptionalToStr{};

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_strutil_h_
