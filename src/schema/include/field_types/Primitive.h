#ifndef SQ_INCLUDE_GUARD_FIELD_TYPES_Primitive_h_
#define SQ_INCLUDE_GUARD_FIELD_TYPES_Primitive_h_

#include <string>
#include <variant>
#include <cstdint>

namespace sq::field_types {

using PrimitiveString = std::string;
using PrimitiveInt = std::int64_t;
using PrimitiveFloat = double;
using PrimitiveBool = bool;

using Primitive = std::variant<
    PrimitiveString,
    PrimitiveInt,
    PrimitiveFloat,
    PrimitiveBool
>;

} // namespace sq::field_types

#endif // SQ_INCLUDE_GUARD_FIELD_TYPES_Primitive_h_
