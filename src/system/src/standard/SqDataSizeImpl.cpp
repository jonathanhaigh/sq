#include "system/standard/SqDataSizeImpl.h"

#include "system/standard/SqFloatImpl.h"
#include "system/standard/SqIntImpl.h"

#include <cstdint>
#include <gsl/narrow>
#include <memory>

namespace sq::system::standard {
namespace {

using Multiplier = std::int64_t;
inline constexpr Multiplier multiplier_k = 1000;
inline constexpr Multiplier multiplier_M = multiplier_k * multiplier_k;
inline constexpr Multiplier multiplier_G = multiplier_M * multiplier_k;
inline constexpr Multiplier multiplier_T = multiplier_G * multiplier_k;
inline constexpr Multiplier multiplier_P = multiplier_T * multiplier_k;
inline constexpr Multiplier multiplier_E = multiplier_P * multiplier_k;

inline constexpr Multiplier multiplier_Ki = 1024;
inline constexpr Multiplier multiplier_Mi = multiplier_Ki * multiplier_Ki;
inline constexpr Multiplier multiplier_Gi = multiplier_Mi * multiplier_Ki;
inline constexpr Multiplier multiplier_Ti = multiplier_Gi * multiplier_Ki;
inline constexpr Multiplier multiplier_Pi = multiplier_Ti * multiplier_Ki;
inline constexpr Multiplier multiplier_Ei = multiplier_Pi * multiplier_Ki;

Result size_in_units(std::size_t size, Multiplier multiplier)
{
    return std::make_unique<SqFloatImpl>(
        gsl::narrow<PrimitiveFloat>(size) / gsl::narrow<PrimitiveFloat>(multiplier)
    );
}

} // namespace

SqDataSizeImpl::SqDataSizeImpl(std::size_t value)
    : value_{value}
{
}

Result SqDataSizeImpl::get_B() const
{
    return std::make_unique<SqIntImpl>(gsl::narrow<PrimitiveInt>(value_));
}

Result SqDataSizeImpl::get_KiB() const
{
    return size_in_units(value_, multiplier_Ki);
}

Result SqDataSizeImpl::get_kB() const
{
    return size_in_units(value_, multiplier_k);
}

Result SqDataSizeImpl::get_MiB() const
{
    return size_in_units(value_, multiplier_Mi);
}

Result SqDataSizeImpl::get_MB() const
{
    return size_in_units(value_, multiplier_M);
}

Result SqDataSizeImpl::get_GiB() const
{
    return size_in_units(value_, multiplier_Gi);
}

Result SqDataSizeImpl::get_GB() const
{
    return size_in_units(value_, multiplier_G);
}

Result SqDataSizeImpl::get_TiB() const
{
    return size_in_units(value_, multiplier_Ti);
}

Result SqDataSizeImpl::get_TB() const
{
    return size_in_units(value_, multiplier_T);
}

Result SqDataSizeImpl::get_PiB() const
{
    return size_in_units(value_, multiplier_Pi);
}

Result SqDataSizeImpl::get_PB() const
{
    return size_in_units(value_, multiplier_P);
}

Result SqDataSizeImpl::get_EiB() const
{
    return size_in_units(value_, multiplier_Ei);
}

Result SqDataSizeImpl::get_EB() const
{
    return size_in_units(value_, multiplier_E);
}

Primitive SqDataSizeImpl::to_primitive() const
{
    return gsl::narrow<PrimitiveInt>(value_);
}

} // namespace sq::system::standard
