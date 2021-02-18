#ifndef SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_
#define SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_

#include "common_types/ArgumentMissingError.h"
#include "common_types/ArgumentTypeError.h"

#include <stdexcept>

namespace sq {

template <typename ParamType>
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

template <typename ParamType>
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

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_
