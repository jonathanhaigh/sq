/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_FieldCallParams_h_
#define SQ_INCLUDE_GUARD_common_types_FieldCallParams_h_

#include "common_types/Primitive.h"

#include "util/typeutil.h"

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

    [[nodiscard]] PosParams& pos_params();
    [[nodiscard]] const PosParams& pos_params() const;

    [[nodiscard]] NamedParams& named_params();
    [[nodiscard]] const NamedParams& named_params() const;

    template <util::Alternative<Primitive> ParamType>
    [[nodiscard]] const ParamType& get(size_t index, std::string_view name) const;

    template <util::Alternative<Primitive> ParamType>
    [[nodiscard]] const ParamType* get_optional(size_t index, std::string_view name) const;

private:

    PosParams pos_params_;
    NamedParams named_params_;
};

std::ostream& operator <<(std::ostream& os, const FieldCallParams& params);
[[nodiscard]] bool operator==(const FieldCallParams& lhs, const FieldCallParams& rhs);
[[nodiscard]] bool operator!=(const FieldCallParams& lhs, const FieldCallParams& rhs);

} // namespace sq

#include "FieldCallParams.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_FieldCallParams_h_
