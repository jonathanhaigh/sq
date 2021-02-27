/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_parser_TokenView_h_
#define SQ_INCLUDE_GUARD_parser_TokenView_h_

#include "common_types/Token.h"

#include <gsl/gsl>
#include <optional>
#include <range/v3/view/facade.hpp>
#include <string_view>

namespace sq::parser {

class TokenView
    : public ranges::view_facade<TokenView>
{
public:
    explicit TokenView(std::string_view str) noexcept;

    TokenView sub_view(gsl::index pos);

    TokenView() noexcept = default;
    TokenView(const TokenView&) noexcept = default;
    TokenView(TokenView&&) noexcept = default;
    TokenView& operator=(const TokenView&) noexcept = default;
    TokenView& operator=(TokenView&&) noexcept = default;
    ~TokenView() noexcept = default;

    friend ranges::range_access;

    [[nodiscard]] const Token& read() const;
    [[nodiscard]] bool equal(ranges::default_sentinel_t other) const noexcept;
    void next();

private:
    gsl::index whitespace_length() const;

    std::string_view str_;
    gsl::index pos_ = 0;
    mutable std::optional<Token> cache_ = std::nullopt;
};

} // namespace sq::parser
#endif // SQ_INCLUDE_GUARD_parser_TokenView_h_
