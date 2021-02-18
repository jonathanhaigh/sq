#ifndef SQ_INCLUDE_GUARD_ast_FilterSpec_h_
#define SQ_INCLUDE_GUARD_ast_FilterSpec_h_

#include <cstddef>
#include <optional>
#include <variant>

#include <iosfwd>

namespace sq::ast {

struct NoFilterSpec { };
std::ostream& operator<<(std::ostream& os, NoFilterSpec nlfs);
[[nodiscard]] bool operator==(NoFilterSpec lhs, NoFilterSpec rhs);
[[nodiscard]] bool operator!=(NoFilterSpec lhs, NoFilterSpec rhs);

struct ElementAccessSpec
{
    std::ptrdiff_t index_;
};
std::ostream& operator<<(std::ostream& os, ElementAccessSpec leas);
[[nodiscard]] bool operator==(ElementAccessSpec lhs, ElementAccessSpec rhs);
[[nodiscard]] bool operator!=(ElementAccessSpec lhs, ElementAccessSpec rhs);

struct SliceSpec
{
    std::optional<std::ptrdiff_t> start_;
    std::optional<std::ptrdiff_t> stop_;
    std::optional<std::ptrdiff_t> step_;
};
std::ostream& operator<<(std::ostream& os, SliceSpec lss);
[[nodiscard]] bool operator==(const SliceSpec& lhs, const SliceSpec& rhs);
[[nodiscard]] bool operator!=(const SliceSpec& lhs, const SliceSpec& rhs);

using FilterSpec = std::variant<
    NoFilterSpec,
    ElementAccessSpec,
    SliceSpec
>;

} // namespace sq::ast

#endif // SQ_INCLUDE_GUARD_ast_FilterSpec_h_
