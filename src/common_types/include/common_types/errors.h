/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_errors_h_
#define SQ_INCLUDE_GUARD_common_types_errors_h_

#include "common_types/Primitive.fwd.h"
#include "common_types/Token.fwd.h"

#include <cstddef>
#include <gsl/gsl>
#include <stdexcept>
#include <string_view>

namespace sq {

/**
 * Base class for errors thrown by the SQ code.
 */
class Exception : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

/**
 * Error indicating that a required parameter of a field is missing.
 */
class ArgumentMissingError : public Exception {
public:
  /**
   * @param arg_name the name of the missing argument.
   * @param arg_type the type of the missing argument.
   */
  ArgumentMissingError(std::string_view arg_name, std::string_view arg_type);
};

/**
 * Error indicating that a given parameter is of incorrect type.
 */
class ArgumentTypeError : public Exception {
public:
  /**
   * @param received the value of the given parameter.
   * @param type_expected the name of the type expected for the parameter.
   */
  ArgumentTypeError(const Primitive &received, std::string_view type_expected);
};

/**
 * Exception indicating a programming error in SQ.
 *
 * InternalError should never actually be thrown - they are used in places that
 * the programmer believes are dead code, but where the C++ language still
 * requires e.g. a return statement.
 */
class InternalError : public Exception {
public:
  using Exception::Exception;
};

class InvalidConversionError : public Exception {
public:
  using Exception::Exception;
  InvalidConversionError(std::string_view from, std::string_view to);
};

/**
 * Error indicating access of a non-existent field.
 */
class InvalidFieldError : public Exception {
public:
  /**
   * @param sq_type the SQ type of the parent of the missing field.
   * @param field the name of the field that was requested.
   *
   * E.g. if the query is "a.b" and "b" is not a field of "a" then sq_type
   * should be the type of "a" and field should be "b".
   */
  InvalidFieldError(std::string_view sq_type, std::string_view field);
};

/**
 * Error indicating incorrect grammar in a query.
 */
class ParseError : public Exception {
public:
  using Exception::Exception;

  /**
   * Create a ParseError for when an unexpected token is found.
   *
   * @param token the unexpected token.
   * @param expecting the set of tokens that would have been valid in place
   * of the unexpected token.
   */
  ParseError(const Token &token, const TokenKindSet &expecting);
};

/**
 * Indicates a failure to interpret part of the input query as a token.
 */
class LexError : public ParseError {
public:
  /**
   * @param pos position in the input query (in characters) at which the lex
   *      error occured.
   * @param query the full input query.
   */
  LexError(gsl::index pos, std::string_view query);
};

/**
 * Error indicating that an array operation has been requested on a non-array
 * type.
 */
class NotAScalarError : public Exception {
public:
  using Exception::Exception;
};

/**
 * Error indicating that an array operation has been requested on a non-array
 * type.
 */
class NotAnArrayError : public Exception {
public:
  using Exception::Exception;
};
/**
 * Error indicating that a requested feature has not been implemented.
 */
class NotImplementedError : public Exception {
public:
  using Exception::Exception;
};

/**
 * Error indicating a request to access an element outside of an allowed range.
 */
class OutOfRangeError : public Exception {
public:
  using Exception::Exception;
  /**
   * @param token the token in the query where the access was requested.
   * @param message details about the requested access.
   */
  OutOfRangeError(const Token &token, std::string_view message);
};

/**
 * Error indicating that a pullup type field access has been requested for a
 * field access with siblings.
 */
class PullupWithSiblingsError : public Exception {
  using Exception::Exception;
};

/**
 * Error indicating that the udev library returned an error.
 */
class UdevError : public Exception {
  using Exception::Exception;
};

} // namespace sq

#endif // SQ_INCLUDE_GUARD_common_types_errors_h_
