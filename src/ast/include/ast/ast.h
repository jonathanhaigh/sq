#ifndef SQ_INCLUDE_GUARD_ast_ast_h_
#define SQ_INCLUDE_GUARD_ast_ast_h_

#include "ast/FilterSpec.h"
#include "common_types/FieldCallParams.h"
#include "util/MoveOnlyTree.h"

#include <iosfwd>
#include <string>

namespace sq::ast {

inline constexpr const char* ast_root_node_name = "root";

class AstData
{
public:
    AstData() = delete;
    AstData(const AstData&) = delete;
    AstData& operator=(const AstData&) = delete;

    AstData(AstData&&) = default;
    AstData& operator=(AstData&&) = default;
    ~AstData() noexcept = default;

    explicit AstData(std::string_view name)
        : name_(name)
    { }

    [[nodiscard]] const std::string& name() const { return name_; }

    [[nodiscard]] const FieldCallParams& params() const { return params_; }
    [[nodiscard]] FieldCallParams& params() { return params_; }

    [[nodiscard]] const FilterSpec& filter_spec() const { return filter_spec_; }
    [[nodiscard]] FilterSpec& filter_spec() { return filter_spec_; }

private:
    std::string name_;
    FieldCallParams params_;
    FilterSpec filter_spec_;
};
std::ostream& operator<<(std::ostream& os, const AstData& ast_data);
[[nodiscard]] bool operator==(const AstData& lhs, const AstData& rhs);
[[nodiscard]] bool operator!=(const AstData& lhs, const AstData& rhs);

using Ast = util::MoveOnlyTree<AstData>;

[[nodiscard]] Ast generate_ast(const std::string& sq_command);

} // namespace sq::ast

#endif // SQ_INCLUDE_GUARD_ast_ast_h_
