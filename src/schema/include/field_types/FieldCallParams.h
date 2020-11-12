#ifndef SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_
#define SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_

#include "Primitive.h"
#include "util/strutil.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <sstream>
#include <string>
#include <string_view>
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

    friend std::ostream& operator <<(std::ostream& os, const FieldCallParams& params)
    {
        // TODO: Something better... Especially when we have ranges or
        // std::ostream_joiner
        auto param_strs = std::vector<std::string>{};
        const auto& pos_params = params.pos_params();
        std::transform(
            std::begin(pos_params),
            std::end(pos_params),
            std::back_inserter(param_strs),
            [](const auto& p) { return util::variant_to_str(p); }
        );

        const auto& named_params = params.named_params();
        std::transform(
            std::begin(named_params),
            std::end(named_params),
            std::back_inserter(param_strs),
            [](const auto& p) {
                std::ostringstream oss;
                oss << p.first << "=" << util::variant_to_str(p.second);
                return oss.str();
            }
        );
        os << util::join(param_strs);
        return os;
    }

    PosParams pos_params_;
    NamedParams named_params_;
};

} // namespace sq::field_types

#endif // SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_
