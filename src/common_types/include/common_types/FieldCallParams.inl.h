/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_
#define SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_

#include "common_types/ArgumentMissingError.h"
#include "common_types/ArgumentTypeError.h"
#include "util/typeutil.h"

#include <stdexcept>

namespace sq {

template <PrimitiveAlternative ParamType>
const ParamType& FieldCallParams::get(size_t index, std::string_view name) const
{
    if (index < pos_params_.size())
    {
        const auto& value = pos_params_[index];
        try
        {
            return std::get<ParamType>(value);
        }
        catch (const std::bad_variant_access&)
        {
            throw ArgumentTypeError{value, primitive_type_name_v<ParamType>};
        }
    }
    try
    {
        const auto& value = named_params_.at(std::string(name));
        try
        {
            return std::get<ParamType>(value);
        }
        catch (const std::bad_variant_access&)
        {
            throw ArgumentTypeError{value, primitive_type_name_v<ParamType>};
        }
    }
    catch(const std::out_of_range&)
    {
        throw ArgumentMissingError{name, primitive_type_name_v<ParamType>};
    }
}

template <PrimitiveAlternative ParamType>
const ParamType* FieldCallParams::get_optional(size_t index, std::string_view name) const
{
    try
    {
        return &(get<ParamType>(index, name));
    }
    catch (const ArgumentMissingError&)
    {
        return nullptr;
    }
}

template <PrimitiveAlternative ParamType>
const ParamType& FieldCallParams::get_or(
    size_t index,
    std::string_view name,
    const ParamType& default_value
) const
{
    try
    {
        return get<ParamType>(index, name);
    }
    catch (const ArgumentMissingError&)
    {
        return default_value;
    }
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_
