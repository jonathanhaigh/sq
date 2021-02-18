#ifndef SQ_INCLUDE_GUARD_system_standard_SqDataSizeImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqDataSizeImpl_h_

#include "system/SqDataSize.gen.h"

#include <cstdint>

namespace sq::system::standard {

class SqDataSizeImpl
    : public SqDataSize<SqDataSizeImpl>
{
public:
    explicit SqDataSizeImpl(std::size_t value);

    [[nodiscard]] Result get_B() const;
    [[nodiscard]] Result get_KiB() const;
    [[nodiscard]] Result get_kB() const;
    [[nodiscard]] Result get_MiB() const;
    [[nodiscard]] Result get_MB() const;
    [[nodiscard]] Result get_GiB() const;
    [[nodiscard]] Result get_GB() const;
    [[nodiscard]] Result get_TiB() const;
    [[nodiscard]] Result get_TB() const;
    [[nodiscard]] Result get_PiB() const;
    [[nodiscard]] Result get_PB() const;
    [[nodiscard]] Result get_EiB() const;
    [[nodiscard]] Result get_EB() const;
    [[nodiscard]] Primitive to_primitive() const override;

private:
    std::size_t value_;
};

} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqDataSizeImpl_h_
