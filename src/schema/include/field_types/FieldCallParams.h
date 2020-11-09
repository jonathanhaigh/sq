#ifndef SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_
#define SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_

#include "Primitive.h"

#include <map>
#include <optional>
#include <string>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace sq::field_types {

class FieldCallParams
{
public:
    using PosParams = std::vector<Primitive>;
    using NamedParams = std::map<std::string, Primitive>;

    FieldCallParams() = default;
    FieldCallParams(const FieldCallParams&) = default;
    FieldCallParams(FieldCallParams&&) = default;
    FieldCallParams& operator=(const FieldCallParams&) = default;
    FieldCallParams& operator=(FieldCallParams&&) = default;
    ~FieldCallParams() noexcept = default;

    PosParams& pos_params() { return pos_params_; }
    const PosParams& pos_params() const { return pos_params_; }

    NamedParams& named_params() { return named_params_; }
    const NamedParams& named_params() const { return named_params_; }

    template <typename ParamType>
    const ParamType& get(const size_t index, const std::string_view name) const
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
    const ParamType* get_optional(const size_t index, const std::string_view name) const
    {
        try
        {
            return &get<ParamType>(index, name);
        }
        catch (const std::out_of_range& e)
        {
            return nullptr;
        }
    }

private:
    PosParams pos_params_;
    NamedParams named_params_;
};

} // namespace sq::field_types

#endif // SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_
