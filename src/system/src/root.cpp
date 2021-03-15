/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/root.h"

#include "system/linux/SqRootImpl.h"

#include <memory>

namespace sq::system {

FieldPtr root() { return std::make_shared<linux::SqRootImpl>(); }

} // namespace sq::system
