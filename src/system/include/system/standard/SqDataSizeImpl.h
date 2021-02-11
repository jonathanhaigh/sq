#ifndef SQ_INCLUDE_GUARD_system_standard_SqDataSizeImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqDataSizeImpl_h_

#include "schema/SqDataSize.gen.h"

#include <cstdint>

namespace sq::system::standard {

class SqDataSizeImpl
    : public schema::SqDataSize<SqDataSizeImpl>
{
public:
    explicit SqDataSizeImpl(std::size_t value);

private:
    friend class schema::SqDataSize<SqDataSizeImpl>;

    Result get_B() const;
    Result get_KiB() const;
    Result get_kB() const;
    Result get_MiB() const;
    Result get_MB() const;
    Result get_GiB() const;
    Result get_GB() const;
    Result get_TiB() const;
    Result get_TB() const;
    Result get_PiB() const;
    Result get_PB() const;
    Result get_EiB() const;
    Result get_EB() const;
    Primitive to_primitive() const;

    std::size_t value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqDataSizeImpl_h_
