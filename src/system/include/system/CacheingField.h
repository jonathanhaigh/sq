/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/
#ifndef SQ_INCLUDE_GUARD_system_CacheingField_h_
#define SQ_INCLUDE_GUARD_system_CacheingField_h_

#include "common_types/Field.h"

#include <map>
#include <string>

namespace sq::system {

class CacheingField
    : public Field
{
public:
    [[nodiscard]] Result get(
        std::string_view member,
        const FieldCallParams& params
    ) const override;

private:
    [[nodiscard]] virtual Result dispatch(
        std::string_view member,
        const FieldCallParams& params
    ) const = 0;

    // NOTE: currently, don't distinguish between field calls made with
    // different parameters. That should be okay because there isn't any
    // query syntax that allows access to the same field with different
    // parameters. That may change in the future though.
    mutable std::map<std::string, Result, std::less<>> cache_;
};

} // namespace sq::system

#endif // SQ_INCLUDE_GUARD_system_CacheingField_h_

