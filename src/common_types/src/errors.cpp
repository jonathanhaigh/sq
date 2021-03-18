/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/errors.h"

#include "common_types/Primitive.h"
#include "common_types/Token.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <string>

namespace sq {

namespace {

/**
 * Get a string showing the position in the query where an error occurred.
 *
 * The position is indicated by printing the full query on one line, and then
 * on the next line, having a "^" in the column where the error is.
 * E.g.:
 * a { query { with { an { error! } } } }
 *                              ^
 */
std::string show_pos_in_query(gsl::index pos, std::string_view query) {
  return fmt::format("{}\n{: >{}}", query, "^", pos + 1);
}

std::string show_pos_in_query(const Token &token) {
  return show_pos_in_query(token.pos(), token.query());
}

} // namespace

ArgumentMissingError::ArgumentMissingError(std::string_view arg_name,
                                           std::string_view arg_type)
    : Exception{
          fmt::format("missing argument {} of type {}", arg_name, arg_type)} {}

ArgumentTypeError::ArgumentTypeError(const Primitive &received,
                                     std::string_view type_expected)
    : Exception{fmt::format("invalid argument {} of type {}, expecting {}",
                            primitive_to_str(received),
                            primitive_type_name(received), type_expected)} {}

InvalidConversionError::InvalidConversionError(std::string_view from,
                                               std::string_view to)
    : Exception{fmt::format("cannot convert {} to {}", from, to)} {}

InvalidFieldError::InvalidFieldError(std::string_view sq_type,
                                     std::string_view field)
    : Exception{fmt::format("{} has no field \"{}\"", sq_type, field)} {}

LexError::LexError(gsl::index pos, std::string_view query)
    : ParseError{
          fmt::format("lex error: failed to identify token at position {}\n{}",
                      pos, show_pos_in_query(pos, query))} {}

OutOfRangeError::OutOfRangeError(const Token &token, std::string_view message)
    : Exception{fmt::format("out of range error at {}: {}\n{}", token, message,
                            show_pos_in_query(token))} {}

ParseError::ParseError(const Token &token, const TokenKindSet &expecting)
    : Exception{fmt::format(
          "parse error: unexpected {}; expecting one of: {}\n{}", token,
          fmt::join(expecting, ", "), show_pos_in_query(token))} {}

FileNotFoundError::FileNotFoundError(const std::filesystem::path &path)
    : FilesystemError{fmt::format("file {} not found", path)} {}

} // namespace sq
