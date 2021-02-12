#ifndef SQ_INCLUDE_GUARD_system_SqRoot_gen_h_
#define SQ_INCLUDE_GUARD_system_SqRoot_gen_h_

#include "common_types/Field.h"

namespace sq::system {

template <typename Impl>
class SqRoot
    : public Field
{
public:
    Result get(std::string_view member, const FieldCallParams& params) const override;
};

} // namespace sq::system

#include "system/SqRoot.gen.inl.h"

#endif // SQ_INCLUDE_GUARD_system_SqRoot_gen_h_