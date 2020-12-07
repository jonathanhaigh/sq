#include "field_types/SqStringImpl.h"

namespace sq::field_types {

SqStringImpl::SqStringImpl(std::string str)
    : string_{std::move(str)}
{ }

} // namespace sq::field_types
