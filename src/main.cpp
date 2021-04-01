/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "core/narrow.h"
#include "core/typeutil.h"
#include "parser/Parser.h"
#include "parser/TokenView.h"
#include "results/Serializer.h"
#include "results/results.h"
#include "system/root.h"

#include <cstddef>
#include <gsl/gsl>
#include <iostream>

namespace {
int run_sq(int argc, char **argv) {
  const auto args = gsl::span{argv, sq::to_size(argc)};
  if (args.size() < 2) {
    std::cerr << "Not enough args\n";
    return 1;
  }
  if (args.size() > 2) {
    std::cerr << "Too many args\n";
    return 1;
  }
  const auto sq_command = std::string{args[1]};
  auto tokens = sq::parser::TokenView{sq_command};
  auto parser = sq::parser::Parser(tokens);
  const auto ast = parser.parse();
  auto serializer = sq::results::get_serializer(std::cout);
  sq::results::generate_results(ast, sq::system::root(), *serializer);

  return 0;
}
} // namespace

int main(int argc, char **argv) {
  try {
    return run_sq(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << sq::base_type_name(e) << ": " << e.what() << std::endl;
    return 1;
  }
}
