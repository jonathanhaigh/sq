/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_strutil_h_
#define SQ_INCLUDE_GUARD_core_strutil_h_

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <optional>
#include <string>
#include <variant>

template <typename... Types>
struct fmt::formatter<std::variant<Types...>> : fmt::formatter<std::string> {

  template <typename FormatContext>
  auto format(const std::variant<Types...> &var, FormatContext &fmt_ctx) {
    auto str =
        std::visit([](const auto &v) { return fmt::format("{}", v); }, var);
    return fmt::formatter<std::string>::format(std::move(str), fmt_ctx);
  }
};

template <typename T>
struct fmt::formatter<std::optional<T>> : fmt::formatter<std::string> {

  template <typename FormatContext>
  auto format(const std::optional<T> &opt, FormatContext &fmt_ctx) {
    auto str = std::string{};
    if (opt) {
      str = fmt::format("{}", opt.value());
    }
    return fmt::formatter<std::string>::format(std::move(str), fmt_ctx);
  }
};

#endif // SQ_INCLUDE_GUARD_core_strutil_h_
