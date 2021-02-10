#ifndef SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_
#define SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_

#include "common_types/SqArgumentTypeError.h"

#include <stdexcept>

namespace sq {

template <typename ParamType>
const ParamType& FieldCallParams::get(const size_t index, const std::string_view name) const
{
    // TODO: better errors

    if (index < pos_params_.size())
    {
        const auto& value = pos_params_[index];
        try
        {
            return std::get<ParamType>(value);
        }
        catch (const std::bad_variant_access&)
        {
            throw SqArgumentTypeError::create<ParamType>(value);
        }
    }
    const auto& value = named_params_.at(std::string(name));
    try
    {
        return std::get<ParamType>(value);
    }
    catch (const std::bad_variant_access&)
    {
        throw SqArgumentTypeError::create<ParamType>(value);
    }
}

template <typename ParamType>
const ParamType* FieldCallParams::get_optional(const size_t index, const std::string_view name) const
{
    try
    {
        return &(get<ParamType>(index, name));
    }
    catch (const std::out_of_range& e)
    {
        return nullptr;
    }
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_
