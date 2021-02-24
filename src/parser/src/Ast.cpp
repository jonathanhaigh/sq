#include "parser/Ast.h"

#include "util/ASSERT.h"
#include "util/strutil.h"

#include <iostream>

namespace sq::parser {

std::ostream& operator<<(std::ostream& os, const AstData& ast_data)
{
    ASSERT(!ast_data.name().empty());
    os << ast_data.name()
       << "(" << ast_data.params() << ")"
       << "[" << util::variant_to_str(ast_data.filter_spec()) << "]";

    return os;
}

bool operator==(const AstData& lhs, const AstData& rhs)
{
    return lhs.name() == rhs.name() &&
           lhs.params() == rhs.params() &&
           lhs.filter_spec() == rhs.filter_spec();
}

bool operator!=(const AstData& lhs, const AstData& rhs)
{
    return !(lhs == rhs);
}

} // namespace sq::parser