/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Field_h_
#define SQ_INCLUDE_GUARD_common_types_Field_h_

#include "common_types/Primitive.h"

#include <memory>
#include <range/v3/view/any_view.hpp>
#include <string_view>
#include <vector>

namespace sq {

class FieldCallParams;
class Field;

using FieldPtr = std::unique_ptr<Field>;
using FieldVector = std::vector<FieldPtr>;

template <ranges::category Cat>
using FieldRange = ranges::any_view<FieldPtr, Cat>;

using Result = std::variant<
    FieldPtr,
    FieldVector,
    FieldRange<ranges::category::input>,
    FieldRange<ranges::category::input | ranges::category::sized>,
    FieldRange<ranges::category::forward>,
    FieldRange<ranges::category::forward | ranges::category::sized>,
    FieldRange<ranges::category::bidirectional>,
    FieldRange<ranges::category::bidirectional | ranges::category::sized>,
    FieldRange<ranges::category::random_access>,
    FieldRange<ranges::category::random_access | ranges::category::sized>
>;

class Field
{
public:
    [[nodiscard]] virtual Result get(std::string_view member, const FieldCallParams& params) const = 0;
    [[nodiscard]] virtual Primitive to_primitive() const = 0;

    Field(const Field&) = delete;
    Field(Field&&) = delete;
    Field& operator=(const Field&) = delete;
    Field& operator=(Field&&) = delete;
    Field() noexcept = default;
    virtual ~Field() noexcept = default;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Field_h_
