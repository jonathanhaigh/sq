/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "core/FieldCallParams.h"

#include "core/strutil.h"

#include <fmt/format.h>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/transform.hpp>

namespace sq {

FieldCallParams::PosParams &FieldCallParams::pos_params() {
  return pos_params_;
}

const FieldCallParams::PosParams &FieldCallParams::pos_params() const {
  return pos_params_;
}

FieldCallParams::NamedParams &FieldCallParams::named_params() {
  return named_params_;
}

const FieldCallParams::NamedParams &FieldCallParams::named_params() const {
  return named_params_;
}

std::ostream &operator<<(std::ostream &os, const FieldCallParams &params) {

  auto named_param_to_str = [](const auto &np) {
    return fmt::format("{}={}", np.first, np.second);
  };

  auto pos = params.pos_params();

  auto named =
      params.named_params() | ranges::views::transform(named_param_to_str);

  auto all = ranges::concat_view(pos, named);

  os << fmt::format("{}", fmt::join(all, ", "));
  return os;
}

} // namespace sq
