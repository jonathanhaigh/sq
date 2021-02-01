#ifndef SQ_INCLUDE_GUARD_results_generate_results_h_
#define SQ_INCLUDE_GUARD_results_generate_results_h_

#include "ast/ast.h"
#include "field_types/Field.h"

#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace sq::results {

class ResultTree {
public:
    using ObjData = std::vector<std::pair<std::string, ResultTree>>;
    using ArrayData = std::vector<ResultTree>;
    using PrimitiveData = field_types::Primitive;
    using Data = std::variant<ObjData, ArrayData, PrimitiveData>;

    ResultTree() = delete;
    ResultTree(const ResultTree&) = delete;
    ResultTree& operator=(const ResultTree&) = delete;

    ResultTree(ResultTree&& other) = default;
    ResultTree& operator=(ResultTree&& other) = default;
    ~ResultTree() noexcept = default;

    explicit ResultTree(const ast::Ast& ast);
    ResultTree(const ast::Ast& ast, field_types::FieldList&& result);
    ResultTree(const ast::Ast& ast, field_types::FieldPtr&& result);
    ResultTree(const ast::Ast& ast, field_types::Primitive&& result);

    const ast::Ast& ast() const { return *ast_; }

    const Data& data() const { return data_; }
    Data& data() { return data_; }

private:
    void from_field_ptr(const ast::Ast& ast, field_types::FieldPtr&& result);
    const ast::Ast* ast_;
    Data data_;
};

ResultTree generate_results(const ast::Ast& ast);

} // namespace sq::results

#endif // SQ_INCLUDE_GUARD_results_generate_results_h_
