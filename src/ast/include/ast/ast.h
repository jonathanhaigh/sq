#ifndef SQ_INCLUDE_GUARD_ast_ast_h_
#define SQ_INCLUDE_GUARD_ast_ast_h_

#include "ast/FilterSpec.h"
#include "field_types/FieldCallParams.h"
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

    const field_types::FieldCallParams& params() const { return params_; }
    field_types::FieldCallParams& params() { return params_; }

    const FilterSpec& filter_spec() const { return filter_spec_; }
    FilterSpec& filter_spec() { return filter_spec_; }

private:
    std::string name_;
    field_types::FieldCallParams params_;
    FilterSpec filter_spec_;
};
std::ostream& operator<<(std::ostream& os, const AstData& ast_data);

using Ast = util::Tree<AstData>;

Ast generate_ast(const std::string& sq_command);

} // namespace sq::ast

#endif // SQ_INCLUDE_GUARD_ast_ast_h_
