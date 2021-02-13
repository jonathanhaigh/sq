#ifndef SQ_INCLUDE_GUARD_results_generate_results_h_
#define SQ_INCLUDE_GUARD_results_generate_results_h_

#include "ast/ast.h"
#include "common_types/Field.h"
#include "common_types/Primitive.h"
#include "results/ResultView.h"

#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace sq::results {

class ResultTree {
public:
    using ObjData = std::vector<std::pair<std::string, ResultTree>>;
    using ArrayData = std::vector<ResultTree>;
    using Data = std::variant<ObjData, ArrayData, Primitive>;

    ResultTree() = delete;
    ResultTree(const ResultTree&) = delete;
    ResultTree& operator=(const ResultTree&) = delete;

    ResultTree(ResultTree&& other) = default;
    ResultTree& operator=(ResultTree&& other) = default;
    ~ResultTree() noexcept = default;

    ResultTree(const ast::Ast& ast, ResultView&& result);
    explicit ResultTree(Data&& data);

    const Data& data() const { return data_; }
    Data& data() { return data_; }

private:
    Data data_;
};

bool operator==(const ResultTree& lhs, const ResultTree& rhs);
bool operator!=(const ResultTree& lhs, const ResultTree& rhs);

} // namespace sq::results

#endif // SQ_INCLUDE_GUARD_results_generate_results_h_
