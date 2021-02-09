#ifndef SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_
#define SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_

#include "Primitive.h"

#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace sq {

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

    PosParams& pos_params();
    const PosParams& pos_params() const;

    NamedParams& named_params();
    const NamedParams& named_params() const;

    template <typename ParamType>
    const ParamType& get(const size_t index, const std::string_view name) const;

    template <typename ParamType>
    const ParamType* get_optional(const size_t index, const std::string_view name) const;

private:

    PosParams pos_params_;
    NamedParams named_params_;
};

std::ostream& operator <<(std::ostream& os, const FieldCallParams& params);
bool operator==(const FieldCallParams& lhs, const FieldCallParams& rhs);
bool operator!=(const FieldCallParams& lhs, const FieldCallParams& rhs);

} // namespace sq

#include "FieldCallParams.inl.h"

#endif // SQ_INCLUDE_GUARD_FIELD_TYPES_FieldCallParams_h_
