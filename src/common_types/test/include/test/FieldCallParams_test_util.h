/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_h_
#define SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_h_

#include "common_types/FieldCallParams.h"
#include "util/typeutil.h"

#include <string_view>

namespace sq::test {

using PosParam = FieldCallParams::PosParams::value_type;
using NamedParam = FieldCallParams::NamedParams::value_type;

template <typename... Args>
[[nodiscard]] FieldCallParams params(Args&&... args);

template <util::ConvertibleToAlternative<Primitive> T>
[[nodiscard]] NamedParam named(std::string_view name, T&& np);

} // namespace sq::test

#include "FieldCallParams_test_util.inl.h"

#endif // SQ_INCLUDE_GUARD_common_types_test_FieldCallParams_test_util_h_
