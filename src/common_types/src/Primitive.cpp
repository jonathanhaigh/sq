#include "common_types/Primitive.h"

namespace sq {
namespace {

struct PrimitiveTypeNameVisitor
{
    template <typename T>
    [[nodiscard]] const char* operator()([[maybe_unused]] const T& value) const
    {
        return primitive_type_name_v<T>;
    }
};

} // namespace

const char* primitive_type_name(const Primitive& value)
{
    return std::visit(PrimitiveTypeNameVisitor{}, value);
}

std::string primitive_to_str(const Primitive& value)
{
    return std::visit(detail::PrimitiveToStrVisitor{}, value);
}

} //namespace sq
