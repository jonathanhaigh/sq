/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "util/typeutil.h"

#include <memory>
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#include <cxxabi.h>
#define SQ_DEMANGLE
#endif

namespace sq::util::detail {

#ifdef SQ_DEMANGLE
std::string demangle(const char *type_name) {
  auto status = int{-1}; // arbitrary non-zero initialization
  const auto demangled = std::unique_ptr<char, void (*)(void *)>{
      abi::__cxa_demangle(type_name, nullptr, nullptr, &status), std::free};
  return (status == 0) ? demangled.get() : type_name;
}
#else
std::string demangle(const char *type_name) { return type_name; }
#endif

} // namespace sq::util::detail
