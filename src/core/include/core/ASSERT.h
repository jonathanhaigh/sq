/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_ASSERT_h_
#define SQ_INCLUDE_GUARD_core_ASSERT_h_

#include <cassert>

// To work around clang-tidy bug: https://reviews.llvm.org/D88833
#define ASSERT assert /* NOLINT */

#endif // SQ_INCLUDE_GUARD_core_ASSERT_h_
