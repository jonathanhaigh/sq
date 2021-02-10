#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_

#include <iomanip>

namespace sq {

template <>
const char* const primitive_type_name_v<PrimitiveString> = "PrimitiveString";

template <>
const char* const primitive_type_name_v<PrimitiveInt> = "PrimitiveInt";

template <>
const char* const primitive_type_name_v<PrimitiveFloat> = "PrimitiveFloat";

template <>
const char* const primitive_type_name_v<PrimitiveBool> = "PrimitiveBool";

namespace detail {

struct PrimitiveTypeNameVisitor
{

    const char* operator()(const Primitive& value) const
    {
        return std::visit(*this, value);
    }

    template <typename T>
    const char* operator()([[maybe_unused]] const T& value) const
    {
        return primitive_type_name_v<T>;
    }
};

struct PrimitiveToStrVisitor
{
    std::string operator()(const Primitive& value)
    {
        return std::visit(*this, value);
    }

    std::string operator()(const PrimitiveString& value)
    {
        os_ << std::quoted(value);
        return os_.str();
    }

    std::string operator()(PrimitiveInt value)
    {
        os_ << value;
        return os_.str();
    }

    std::string operator()(PrimitiveBool value)
    {
        os_ << std::boolalpha << value;
        return os_.str();
    }

    std::string operator()(PrimitiveFloat value)
    {
        os_ << value;
        return os_.str();
    }

private:
    std::ostringstream os_;
};


} // namespace detail

template <typename T>
const char* primitive_type_name(const T& value)
{
    return detail::PrimitiveTypeNameVisitor{}(value);
}

template <typename T>
std::string primitive_to_str(const T& value)
{
    return detail::PrimitiveToStrVisitor{}(value);
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_
