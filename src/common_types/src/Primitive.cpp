/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/Primitive.h"

#include "util/typeutil.h"

namespace sq {
namespace {

struct PrimitiveTypeNameVisitor
{
    template <util::Alternative<Primitive> T>
    [[nodiscard]] std::string_view operator()([[maybe_unused]] const T& value) const
    {
        return primitive_type_name_v<T>;
    }
};

} // namespace

std::string_view primitive_type_name(const Primitive& value)
{
    return std::visit(PrimitiveTypeNameVisitor{}, value);
}

std::string primitive_to_str(const Primitive& value)
{
    return std::visit(detail::PrimitiveToStrVisitor{}, value);
}

} //namespace sq
