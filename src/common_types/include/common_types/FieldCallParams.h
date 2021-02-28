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

/**
 * Represents parameters given when accessing a field of a system object.
 */
class FieldCallParams
{
public:
    using PosParams = std::vector<Primitive>;
    using NamedParams = std::map<std::string, Primitive>;

    /**
     * Create an empty set of parameters.
     */
    FieldCallParams() = default;

    FieldCallParams(const FieldCallParams&) = default;
    FieldCallParams(FieldCallParams&&) = default;
    FieldCallParams& operator=(const FieldCallParams&) = default;
    FieldCallParams& operator=(FieldCallParams&&) = default;
    ~FieldCallParams() noexcept = default;

    ///@{
    /**
     * Get the positional parameters for the field access.
     */
    [[nodiscard]] PosParams& pos_params();
    [[nodiscard]] const PosParams& pos_params() const;
    ///@}

    ///@{
    /**
     * Get the named parameters for the field access
     */
    [[nodiscard]] NamedParams& named_params();
    [[nodiscard]] const NamedParams& named_params() const;
    ///@}

    /**
     * Get a required parameter given its name, index and type.
     *
     * If the parameter is not present then an ArgumentMissingError is thrown.
     * If the parameter is present but is not of the requested type then an
     * ArgumentTypeError is thrown.
     */
    template <util::Alternative<Primitive> ParamType>
    [[nodiscard]] const ParamType& get(size_t index, std::string_view name) const;

    /**
     * Get an optional parameter given its name, index and type.
     *
     * Returns nullptr if the parameter is not present. Throws
     * ArgumentTypeError if the parameter is present but is not of the
     * requested type.
     */
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
