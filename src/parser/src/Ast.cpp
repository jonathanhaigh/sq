/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "parser/Ast.h"

#include "util/ASSERT.h"
#include "util/strutil.h"

#include <gsl/gsl>
#include <iostream>

namespace sq::parser {

namespace {

const char *field_access_type_to_str(FieldAccessType access_type) {
  switch (access_type) {
  case FieldAccessType::Default:
    return "Default";
  case FieldAccessType::Pullup:
    return "Pullup";
  }
  ASSERT(0);
  return "Unrecognized FieldAccessType";
}

} // namespace

std::ostream &operator<<(std::ostream &os, const FieldAccessType &access_type) {
  os << field_access_type_to_str(access_type);
  return os;
}

std::ostream &operator<<(std::ostream &os, const AstData &ast_data) {
  Expects(!ast_data.name().empty());
  os << ast_data.name() << "[" << ast_data.access_type() << "]("
     << ast_data.params() << ")"
     << "[" << util::variant_to_str(ast_data.filter_spec()) << "]";

  return os;
}

} // namespace sq::parser
