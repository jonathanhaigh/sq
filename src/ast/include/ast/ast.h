#ifndef SQ_INCLUDE_GUARD_ast_ast_h_
#define SQ_INCLUDE_GUARD_ast_ast_h_

#include "util/Tree.h"

#include <iosfwd>
#include <string>

namespace sq::ast {

class AstData
{
public:
    AstData() = delete;
    AstData(const AstData&) = delete;
    AstData& operator=(const AstData&) = delete;

    AstData(AstData&&) = default;
    AstData& operator=(AstData&&) = default;
    ~AstData() noexcept = default;

    template <typename String>
    explicit AstData(String&& name)
        : name_(std::forward<String>(name))
    { }

    const std::string& name() const { return name_; }
    const std::vector<std::string>& positional_parameters() const { return pos_params_; }
    std::vector<std::string>& positional_parameters() { return pos_params_; }

private:
    std::string name_;
    std::vector<std::string> pos_params_;
};
std::ostream& operator<<(std::ostream& os, const AstData& ast_data);

using Ast = util::Tree<AstData>;

Ast generate_ast(const std::string& sq_command);

} // namespace sq::ast

#endif // SQ_INCLUDE_GUARD_ast_ast_h_
