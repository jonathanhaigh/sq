#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_h_

#include <string>
#include <variant>
#include <cstdint>

namespace sq {

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

template <typename T>
inline constexpr const char* primitive_type_name_v = nullptr;

template <typename T>
static const char* primitive_type_name(const T& value);

template <typename T>
std::string primitive_to_str(const T& value);

} // namespace sq

#include "Primitive.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_h_
