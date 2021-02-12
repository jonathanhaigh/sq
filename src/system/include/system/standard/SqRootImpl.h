#ifndef SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_

#include "system/SqRoot.gen.h"

namespace sq::system::standard {

class SqRootImpl
    : public SqRoot<SqRootImpl>
{
private:
    friend class SqRoot<SqRootImpl>;

    static Result get_path(const PrimitiveString* path);
    static Result get_int(const PrimitiveInt* value);
    static Result get_ints(const PrimitiveInt* start, const PrimitiveInt* stop);
    static Result get_bool(const PrimitiveBool* value);
    Primitive to_primitive() const override;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqRootImpl_h_
