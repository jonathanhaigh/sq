#ifndef SQ_INCLUDE_GUARD_ast_ListFilterSpec_h_
#define SQ_INCLUDE_GUARD_ast_ListFilterSpec_h_

#include <cstddef>
#include <optional>
#include <variant>

namespace sq::ast {

struct NoListFilterSpec { };
std::ostream& operator<<(std::ostream& os, NoListFilterSpec nlfs);

struct ListElementAccessSpec
{
    std::ptrdiff_t index_;
};
std::ostream& operator<<(std::ostream& os, ListElementAccessSpec leas);

struct ListSliceSpec
{
    std::optional<std::ptrdiff_t> start_;
    std::optional<std::ptrdiff_t> stop_;
    std::optional<std::ptrdiff_t> step_;
};
std::ostream& operator<<(std::ostream& os, ListSliceSpec lss);

using ListFilterSpec = std::variant<
    NoListFilterSpec,
    ListElementAccessSpec,
    ListSliceSpec
>;

} // namespace sq::ast

#endif // SQ_INCLUDE_GUARD_ast_ListFilterSpec_h_
