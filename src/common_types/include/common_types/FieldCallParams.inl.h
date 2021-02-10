#ifndef SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_
#define SQ_INCLUDE_GUARD_common_types_FieldCallParams_inl_h_

#include <stdexcept>

namespace sq {

template <typename ParamType>
const ParamType& FieldCallParams::get(const size_t index, const std::string_view name) const
{
    if (index < pos_params_.size())
    {
        return std::get<ParamType>(pos_params_[index]);
        // TODO: better errors
    }
    return std::get<ParamType>(named_params_.at(std::string(name)));
        // TODO: better errors
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
