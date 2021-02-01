#ifndef SQ_INCLUDE_GUARD_FIELD_TYPES_Field_h_
#define SQ_INCLUDE_GUARD_FIELD_TYPES_Field_h_

#include "field_types/FieldCallParams.h"
#include "field_types/Primitive.h"

#include <memory>
#include <range/v3/view/any_view.hpp>
#include <string_view>
#include <vector>

namespace sq {
namespace field_types {

class Field;

using FieldPtr = std::unique_ptr<Field>;
using FieldList = std::vector<FieldPtr>;

using FieldInputRange = ranges::any_view<
    FieldPtr, ranges::category::input
>;

using Result = std::variant<
    FieldPtr,
    FieldList,
    FieldInputRange
>;

class Field
{
public:
    virtual ~Field() noexcept = default;

    virtual Result get(std::string_view member, const FieldCallParams& params) const = 0;
    virtual Primitive to_primitive() const = 0;

protected:
    Field() = default;
};

} // namespace field_types
} // namespace sq

#endif // SQ_INCLUDE_GUARD_FIELD_TYPES_Field_h_
