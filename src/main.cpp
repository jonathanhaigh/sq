#include "ast/ast.h"
#include "results/results.h"
#include "serialization/serialize.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Not enough args\n";
        return 1;
    }
    if (argc > 2) {
        std::cerr << "Too many args\n";
        return 1;
    }
    const auto sq_command = std::string{argv[1]};
    const auto ast = sq::ast::generate_ast(sq_command);
    std::cerr << "AST:\n";
    std::cerr << ast << '\n';
    const auto results = sq::results::generate_results(ast);

    sq::serialization::serialize_results(std::cerr, results);

    return 0;
}
