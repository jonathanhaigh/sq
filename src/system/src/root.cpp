/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/root.h"

#include "system/standard/SqRootImpl.h"

#include <memory>

namespace sq::system {

FieldPtr root() { return std::make_shared<standard::SqRootImpl>(); }

} // namespace sq::system
