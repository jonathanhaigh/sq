/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "common_types/FieldCallParams.h"

#include "util/strutil.h"

#include <range/v3/view/concat.hpp>
#include <range/v3/view/transform.hpp>
#include <sstream>

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
    std::ostringstream oss;
    oss << np.first << "=" << util::variant_to_str(np.second);
    return oss.str();
  };

  auto pos =
      params.pos_params() | ranges::views::transform(util::variant_to_str);

  auto named =
      params.named_params() | ranges::views::transform(named_param_to_str);

  auto all = ranges::concat_view(pos, named);

  auto joined = util::join(all, ", ");

  os << joined;
  return os;
}

} // namespace sq
