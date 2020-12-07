#include "ast/ast.h"
#include "results/results.h"
#include "serialization/serialize.h"

#include <cstddef>
#include <gsl/narrow>
#include <gsl/span>
#include <iostream>

static int run_sq(int argc, char** argv)
{
    const auto args = gsl::span{argv, gsl::narrow<std::size_t>(argc)};
    if (args.size() < 2) {
        std::cerr << "Not enough args\n";
        return 1;
    }
    if (args.size() > 2) {
        std::cerr << "Too many args\n";
        return 1;
    }
    const auto sq_command = std::string{args[1]};
    const auto ast = sq::ast::generate_ast(sq_command);
    std::cerr << "AST:\n";
    std::cerr << ast << '\n';
    const auto results = sq::results::generate_results(ast);

    sq::serialization::serialize_results(std::cerr, results);

    return 0;
}

int main(int argc, char** argv)
{
    try {
        return run_sq(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
