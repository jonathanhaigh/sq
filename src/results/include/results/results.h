/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_results_generate_results_h_
#define SQ_INCLUDE_GUARD_results_generate_results_h_

#include "core/Field.h"
#include "parser/Ast.h"

namespace sq::results {

class Serializer;

void generate_results(const parser::Ast &ast, const FieldPtr &system_root,
                      Serializer &serializer);

} // namespace sq::results

#endif // SQ_INCLUDE_GUARD_results_generate_results_h_
