/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_util_strutil_h_
#define SQ_INCLUDE_GUARD_util_strutil_h_

#include "util/typeutil.h"

#include <gsl/gsl>
#include <iostream>
#include <memory>
#include <range/v3/range/access.hpp>
#include <sstream>
#include <string>
#include <utility>
#include <variant>

namespace sq::util {

template<ranges::cpp20::range R>
    requires Printable<ranges::range_value_t<R>>
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
    template <Printable... Types>
    [[nodiscard]] std::string operator()(const std::variant<Types...>& var) const
    {
        auto ss = std::ostringstream{};
        std::visit(
            [&ss](auto&& v) { ss << v; },
            var
        );
        return ss.str();
    }
};

struct OptionalToStr
{
    template <Printable T>
    [[nodiscard]] std::string operator()(const std::optional<T>& opt) const
    {
        if (opt)
        {
            auto ss = std::ostringstream{};
            ss << opt.value();
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
