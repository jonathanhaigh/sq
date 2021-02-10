#include "field_types/SqDataSize.gen.h"

#include "field_types/SqFloat.gen.h"
#include "field_types/SqInt.gen.h"

#include <cstdint>
#include <gsl/narrow>

namespace sq::field_types {

using Multiplier = std::int64_t;
static constexpr Multiplier multiplier_k = 1000;
static constexpr Multiplier multiplier_M = multiplier_k * multiplier_k;
static constexpr Multiplier multiplier_G = multiplier_M * multiplier_k;
static constexpr Multiplier multiplier_T = multiplier_G * multiplier_k;
static constexpr Multiplier multiplier_P = multiplier_T * multiplier_k;
static constexpr Multiplier multiplier_E = multiplier_P * multiplier_k;

static constexpr Multiplier multiplier_Ki = 1024;
static constexpr Multiplier multiplier_Mi = multiplier_Ki * multiplier_Ki;
static constexpr Multiplier multiplier_Gi = multiplier_Mi * multiplier_Ki;
static constexpr Multiplier multiplier_Ti = multiplier_Gi * multiplier_Ki;
static constexpr Multiplier multiplier_Pi = multiplier_Ti * multiplier_Ki;
static constexpr Multiplier multiplier_Ei = multiplier_Pi * multiplier_Ki;

static Result size_in_units(const std::size_t size, const Multiplier multiplier)
{
    return SqFloat::create(
        gsl::narrow<PrimitiveFloat>(size) / gsl::narrow<PrimitiveFloat>(multiplier)
    );
}

Result SqDataSize::get_B() const
{
    return SqInt::create(gsl::narrow<PrimitiveInt>(impl_));
}

Result SqDataSize::get_KiB() const
{
    return size_in_units(impl_, multiplier_Ki);
}

Result SqDataSize::get_kB() const
{
    return size_in_units(impl_, multiplier_k);
}

Result SqDataSize::get_MiB() const
{
    return size_in_units(impl_, multiplier_Mi);
}

Result SqDataSize::get_MB() const
{
    return size_in_units(impl_, multiplier_M);
}

Result SqDataSize::get_GiB() const
{
    return size_in_units(impl_, multiplier_Gi);
}

Result SqDataSize::get_GB() const
{
    return size_in_units(impl_, multiplier_G);
}

Result SqDataSize::get_TiB() const
{
    return size_in_units(impl_, multiplier_Ti);
}

Result SqDataSize::get_TB() const
{
    return size_in_units(impl_, multiplier_T);
}

Result SqDataSize::get_PiB() const
{
    return size_in_units(impl_, multiplier_Pi);
}

Result SqDataSize::get_PB() const
{
    return size_in_units(impl_, multiplier_P);
}

Result SqDataSize::get_EiB() const
{
    return size_in_units(impl_, multiplier_Ei);
}

Result SqDataSize::get_EB() const
{
    return size_in_units(impl_, multiplier_E);
}

Primitive SqDataSize::to_primitive() const
{
    return gsl::narrow<PrimitiveInt>(impl_);
}

} // namespace sq::field_types
