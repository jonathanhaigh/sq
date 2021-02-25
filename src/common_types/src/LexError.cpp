/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/LexError.h"

#include "util/typeutil.h"

#include <sstream>
#include <string>

namespace sq {

namespace {

[[nodiscard]] std::string lex_error_message(
    std::ptrdiff_t pos,
    std::string_view query
)
{
    auto ss = std::stringstream{};
    ss << "lex error: Failed to identify token at position " << pos
       << "\n" << query
       << std::string(util::to_size_t(pos), ' ') << "^\n";

    return ss.str();
}

} // namespace

LexError::LexError(std::ptrdiff_t pos, std::string_view query)
    : ParseError(lex_error_message(pos, query))
{
}

} // namespace sq
