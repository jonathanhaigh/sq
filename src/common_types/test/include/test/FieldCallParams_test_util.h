#ifndef SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_h_
#define SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_h_

#include "common_types/FieldCallParams.h"

namespace sq::test {

using PosParam = FieldCallParams::PosParams::value_type;
using NamedParam = FieldCallParams::NamedParams::value_type;

static const auto no_params = FieldCallParams{};

template <typename... Args>
FieldCallParams params(Args&&... args);

template <typename T>
NamedParam named(const char* name, T&& np);

} // namespace sq::test

#include "FieldCallParams_test_util.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_h_
