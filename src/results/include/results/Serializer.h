/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_results_Serializer_h_
#define SQ_INCLUDE_GUARD_results_Serializer_h_

#include "results/results.h"

#include <iosfwd>
#include <memory>
#include <string_view>

namespace sq::results {

class Serializer {
public:
  Serializer(const Serializer &) = delete;
  Serializer(Serializer &&) = delete;
  Serializer &operator=(const Serializer &) = delete;
  Serializer &operator=(Serializer &&) = delete;

  Serializer() = default;
  virtual ~Serializer() noexcept = default;

  virtual void start_array() = 0;
  virtual void end_array() = 0;
  virtual void start_object() = 0;
  virtual void end_object() = 0;
  virtual void write_key(std::string_view key) = 0;
  virtual void write_value(const Primitive &value) = 0;
};

std::unique_ptr<Serializer> get_serializer(std::ostream &os);

} // namespace sq::results

#endif // SQ_INCLUDE_GUARD_results_Serializer_h_
