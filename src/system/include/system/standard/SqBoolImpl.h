#ifndef SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_

#include "system/SqBool.gen.h"

namespace sq::system::standard {

class SqBoolImpl
    : public SqBool<SqBoolImpl>
{
public:
    explicit SqBoolImpl(PrimitiveBool value);

    [[nodiscard]] Primitive to_primitive() const override;

private:
    PrimitiveBool value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqBoolImpl_h_
