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

/**
 * The result of accessing a field of a system object.
 *
 * * FieldPtr is used when a single result is returned.
 *
 * * FieldVector is used when multiple results are returned and it is not
 *   possible or desirable to generate results on demand using a generator.
 *   E.g. a list of elements that is small in size.
 *
 * * FieldRange<*> is used when multiple results are returned and it is
 *   possible to generate those results on-demand rather than having to keep
 *   all of the results in memory at once.
 */
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

/**
 * Represents a system object.
 */
class Field
{
public:
    /**
     * Access a field of the system object.
     *
     * This method is implemented in subclasses generated automatically from
     * the system schema. Those subclasses (in the sq_system_dispatch library)
     * dispatch the call via the CRTP to derived implementation classes that
     * implement get_<field_name>() methods.
     *
     * @param member the name of the field to access.
     * @param params parameters to pass to system when accessing the field.
     */
    [[nodiscard]] virtual Result get(std::string_view member, const FieldCallParams& params) const = 0;

    /**
     * Get a representation of the system object as a Primitive type.
     */
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
