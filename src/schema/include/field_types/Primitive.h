#ifndef SQ_INCLUDE_GUARD_FIELD_TYPES_Primitive_h_
#define SQ_INCLUDE_GUARD_FIELD_TYPES_Primitive_h_

#include <string>
#include <variant>

namespace sq {
namespace field_types {

using PrimitiveString = std::string;

using Primitive = std::variant<
    PrimitiveString
>;

} // namespace field_types
} // namespace sq

#endif // SQ_INCLUDE_GUARD_FIELD_TYPES_Primitive_h_
