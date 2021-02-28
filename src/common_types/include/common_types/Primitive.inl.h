/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_

#include <iomanip>
#include <sstream>
#include <string_view>

#include "util/typeutil.h"

namespace sq {

template <util::Alternative<Primitive> P>
struct PrimitiveTypeName
{ };

template <>
struct PrimitiveTypeName<PrimitiveString>
{
    static constexpr std::string_view value = "PrimitiveString";
};

template <>
struct PrimitiveTypeName<PrimitiveInt>
{
    static constexpr std::string_view value = "PrimitiveInt";
};

template <>
struct PrimitiveTypeName<PrimitiveFloat>
{
    static constexpr std::string_view value = "PrimitiveFloat";
};

template <>
struct PrimitiveTypeName<PrimitiveBool>
{
    static constexpr std::string_view value = "PrimitiveBool";
};

namespace detail {

struct PrimitiveToStrVisitor
{
    [[nodiscard]] std::string operator()(const PrimitiveString& value)
    {
        os_ << std::quoted(value);
        return os_.str();
    }

    [[nodiscard]] std::string operator()(PrimitiveBool value)
    {
        os_ << std::boolalpha << value;
        return os_.str();
    }

    template <util::Alternative<Primitive> T>
    [[nodiscard]] std::string operator()(const T& value)
    {
        os_ << value;
        return os_.str();
    }

private:
    std::ostringstream os_;
};

} // namespace detail

template <util::Alternative<Primitive> T>
std::string primitive_to_str(const T& value)
{
    return detail::PrimitiveToStrVisitor{}(value);
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_
