/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_
#define SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_

#include "util/typeutil.h"

#include <iomanip>
#include <sstream>
#include <string_view>

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

} // namespace detail

std::string primitive_to_str(const PrimitiveAlternative auto &value) {
  return detail::PrimitiveToStrVisitor{}(value);
}

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_Primitive_inl_h_
