/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_core_FieldCallParams_h_
#define SQ_INCLUDE_GUARD_core_FieldCallParams_h_

#include "core/Primitive.h"
#include "core/typeutil.h"

#include <compare>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace sq {

/**
 * Represents parameters given when accessing a field of a system object.
 */
class FieldCallParams {
public:
  using PosParams = std::vector<Primitive>;
  using NamedParams = std::map<std::string, Primitive>;

  /**
   * Create an empty set of parameters.
   */
  FieldCallParams() = default;

  FieldCallParams(const FieldCallParams &) = default;
  FieldCallParams(FieldCallParams &&) = default;
  FieldCallParams &operator=(const FieldCallParams &) = default;
  FieldCallParams &operator=(FieldCallParams &&) = default;
  ~FieldCallParams() noexcept = default;

  ///@{
  /**
   * Get the positional parameters for the field access.
   */
  SQ_ND PosParams &pos_params();
  SQ_ND const PosParams &pos_params() const;
  ///@}

  ///@{
  /**
   * Get the named parameters for the field access
   */
  SQ_ND NamedParams &named_params();
  SQ_ND const NamedParams &named_params() const;
  ///@}

  /**
   * Get a required parameter given its name, index and type.
   *
   * If the parameter is not present then an ArgumentMissingError is thrown.
   * If the parameter is present but is not of the requested type then an
   * ArgumentTypeError is thrown.
   */
  template <PrimitiveAlternative ParamType>
  SQ_ND ParamType get(size_t index, std::string_view name) const;

  /**
   * Get an optional parameter given its name, index and type.
   *
   * Returns nullptr if the parameter is not present. Throws
   * ArgumentTypeError if the parameter is present but is not of the
   * requested type.
   */
  template <PrimitiveAlternative ParamType>
  SQ_ND std::optional<ParamType> get_optional(size_t index,
                                              std::string_view name) const;

  /**
   * Get an optional parameter if it is present, else return a default value.
   *
   * Throws ArgumentTypeError if the parameter is present but is not of the
   * requested type.
   */
  template <PrimitiveAlternative ParamType>
  SQ_ND ParamType get_or(size_t index, std::string_view name,
                         const ParamType &default_value) const;

  SQ_ND auto operator<=>(const FieldCallParams &rhs) const = default;

private:
  PosParams pos_params_;
  NamedParams named_params_;
};

std::ostream &operator<<(std::ostream &os, const FieldCallParams &params);

} // namespace sq

#include "FieldCallParams.inl.h"

#endif // SQ_INCLUDE_GUARD_core_FieldCallParams_h_
