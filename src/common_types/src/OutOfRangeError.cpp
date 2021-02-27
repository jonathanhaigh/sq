/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/OutOfRangeError.h"

#include "common_types/Token.h"
#include "util/strutil.h"
#include "util/typeutil.h"

#include <sstream>

namespace sq {

namespace {

[[nodiscard]] std::string create_oor_message(
    const Token& token,
    std::string_view message
)
{
    auto ss = std::ostringstream{};
    ss << "OutOfRangeError at " << token
       << ": " << message
       << "\n" << token.query()
       << "\n" << std::string(util::to_size(token.pos()), ' ') << "^\n";
    return ss.str();
}

} // namespace

OutOfRangeError::OutOfRangeError(const Token& token, std::string_view message)
    : OutOfRangeError{create_oor_message(token, message)}
{
}

} // namespace sq
