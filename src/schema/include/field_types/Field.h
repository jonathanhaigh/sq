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
    virtual ~Field() noexcept = default;

    virtual Result get(std::string_view member, const FieldCallParams& params) const = 0;
    virtual Primitive to_primitive() const = 0;

protected:
    Field() = default;
};

} // namespace field_types

using FieldPtr = field_types::FieldPtr;
using FieldVector = field_types::FieldVector;

template <ranges::category Cat>
using FieldRange = field_types::FieldRange<Cat>;

using Result = field_types::Result;

} // namespace sq

#endif // SQ_INCLUDE_GUARD_FIELD_TYPES_Field_h_
