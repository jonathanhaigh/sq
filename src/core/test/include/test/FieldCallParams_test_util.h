/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_test_FieldCallParams_test_util_h_
#define SQ_INCLUDE_GUARD_core_test_FieldCallParams_test_util_h_

#include "core/FieldCallParams.h"
#include "core/typeutil.h"

#include <string_view>

namespace sq::test {

using PosParam = FieldCallParams::PosParams::value_type;
using NamedParam = FieldCallParams::NamedParams::value_type;

SQ_ND FieldCallParams params(auto &&...args);
SQ_ND NamedParam named(std::string_view name, PrimitiveLike auto &&np);

} // namespace sq::test

#include "FieldCallParams_test_util.inl.h"

#endif // SQ_INCLUDE_GUARD_core_test_FieldCallParams_test_util_h_
