#include "field_types/FieldCallParams.h"

#include "util/strutil.h"

#include <algorithm>
#include <iterator>
#include <sstream>

namespace sq
{

FieldCallParams::PosParams& FieldCallParams::pos_params()
{
    return pos_params_;
}

const FieldCallParams::PosParams& FieldCallParams::pos_params() const
{
    return pos_params_;
}

FieldCallParams::NamedParams& FieldCallParams::named_params()
{
    return named_params_;
}

const FieldCallParams::NamedParams& FieldCallParams::named_params() const
{
    return named_params_;
}

std::ostream& operator <<(std::ostream& os, const FieldCallParams& params)
{
    // TODO: Something better... Especially when we have ranges or
    // std::ostream_joiner
    auto param_strs = std::vector<std::string>{};
    const auto& pos_params = params.pos_params();
    std::transform(
        std::begin(pos_params),
        std::end(pos_params),
        std::back_inserter(param_strs),
        [](const auto& p) { return util::variant_to_str(p); }
    );

    const auto& named_params = params.named_params();
    std::transform(
        std::begin(named_params),
        std::end(named_params),
        std::back_inserter(param_strs),
        [](const auto& p) {
            std::ostringstream oss;
            oss << p.first << "=" << util::variant_to_str(p.second);
            return oss.str();
        }
    );
    os << util::join(param_strs);
    return os;
}

bool operator==(const FieldCallParams& lhs, const FieldCallParams& rhs)
{
    return lhs.pos_params() == rhs.pos_params() &&
           lhs.named_params() == rhs.named_params();
}
bool operator!=(const FieldCallParams& lhs, const FieldCallParams& rhs)
{
    return !(lhs == rhs);
}

} // namespace sq
