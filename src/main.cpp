#include "ast/ast.h"
#include "results/results.h"
#include "serialization/serialize.h"
#include "system/root.h"

#include <cstddef>
#include <gsl/narrow>
#include <gsl/span>
#include <iostream>

namespace {
int run_sq(int argc, char** argv)
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
    auto root = sq::results::ResultView{sq::system::root()};
    const auto results = sq::results::ResultTree(ast, std::move(root));

    sq::serialization::serialize_results(std::cerr, results);

    return 0;
}
} // namespace

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
