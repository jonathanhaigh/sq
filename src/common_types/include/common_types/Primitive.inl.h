/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_

#include <iomanip>
#include <sstream>
#include <string_view>

namespace sq {

namespace detail {

template <typename T>
struct PrimitiveTraits
{
    static constexpr bool is_primitive = false;
};

template <>
struct PrimitiveTraits<PrimitiveString>
{
    static constexpr bool is_primitive = true;
    static constexpr std::string_view name = "PrimitiveString";
};

template <>
struct PrimitiveTraits<PrimitiveInt>
{
    static constexpr bool is_primitive = true;
    static constexpr std::string_view name = "PrimitiveInt";
};

template <>
struct PrimitiveTraits<PrimitiveFloat>
{
    static constexpr bool is_primitive = true;
    static constexpr std::string_view name = "PrimitiveFloat";
};

template <>
struct PrimitiveTraits<PrimitiveBool>
{
    static constexpr bool is_primitive = true;
    static constexpr std::string_view name = "PrimitiveBool";
};

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

    template <typename T>
    [[nodiscard]] std::string operator()(const T& value)
    {
        os_ << value;
        return os_.str();
    }

private:
    std::ostringstream os_;
};

} // namespace detail

template <typename T, typename>
std::string primitive_to_str(const PrimitiveString& value)
{
    return detail::PrimitiveToStrVisitor{}(value);
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_
