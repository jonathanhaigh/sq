/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_Primitive_inl_h_
#define SQ_INCLUDE_GUARD_core_Primitive_inl_h_

#include "core/errors.h"
#include "core/narrow.h"
#include "core/typeutil.h"

#include <gsl/gsl>
#include <iomanip>
#include <sstream>
#include <string_view>
#include <type_traits>

namespace sq {

template <PrimitiveAlternative P> struct PrimitiveTypeName {};

template <> struct PrimitiveTypeName<PrimitiveString> {
  static constexpr std::string_view value = "PrimitiveString";
};

template <> struct PrimitiveTypeName<PrimitiveInt> {
  static constexpr std::string_view value = "PrimitiveInt";
};

template <> struct PrimitiveTypeName<PrimitiveFloat> {
  static constexpr std::string_view value = "PrimitiveFloat";
};

template <> struct PrimitiveTypeName<PrimitiveBool> {
  static constexpr std::string_view value = "PrimitiveBool";
};

template <> struct PrimitiveTypeName<PrimitiveNull> {
  static constexpr std::string_view value = "PrimitiveNull";
};

constexpr PrimitiveInt to_primitive_int(auto value, auto &&...format_args) {
  return narrow<PrimitiveInt>(SQ_FWD(value), SQ_FWD(format_args)...);
}

constexpr PrimitiveFloat to_primitive_float(auto value, auto &&...format_args) {
  return narrow<PrimitiveFloat>(SQ_FWD(value), SQ_FWD(format_args)...);
}

namespace detail {

struct PrimitiveToStrVisitor {
  SQ_ND std::string operator()(const PrimitiveString &value) {
    os_ << std::quoted(value);
    return os_.str();
  }

  SQ_ND std::string operator()(PrimitiveBool value) {
    os_ << std::boolalpha << value;
    return os_.str();
  }

  SQ_ND std::string operator()(const PrimitiveAlternative auto &value) {
    os_ << value;
    return os_.str();
  }

private:
  std::ostringstream os_;
};

template <PrimitiveAlternative T> struct ConvertPrimitiveBase {
  // We can always convert T to T
  SQ_ND T operator()(const T &value) { return value; }

  // By default, don't allow any other conversions
  template <PrimitiveAlternative U> SQ_ND T operator()(SQ_MU const U &value) {
    throw InvalidConversionError{primitive_type_name_v<U>,
                                 primitive_type_name_v<T>};
  }
};

template <PrimitiveAlternative T>
struct ConvertPrimitive : ConvertPrimitiveBase<T> {
  using ConvertPrimitiveBase<T>::operator();
};

template <>
struct ConvertPrimitive<PrimitiveFloat> : ConvertPrimitiveBase<PrimitiveFloat> {
  using ConvertPrimitiveBase<PrimitiveFloat>::operator();

  // Allow PrimitiveInt -> PrimitiveFloat conversions
  SQ_ND PrimitiveFloat operator()(const PrimitiveInt &value) {
    return to_primitive_float(value);
  }
};

} // namespace detail

std::string primitive_to_str(const PrimitiveAlternative auto &value) {
  return detail::PrimitiveToStrVisitor{}(value);
}

template <PrimitiveAlternative T> T convert_primitive(const Primitive &value) {
  return std::visit(detail::ConvertPrimitive<T>{}, value);
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_core_Primitive_inl_h_
