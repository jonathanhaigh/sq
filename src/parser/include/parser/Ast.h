/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_parser_Ast_h_
#define SQ_INCLUDE_GUARD_parser_Ast_h_

#include "core/FieldCallParams.h"
#include "core/MoveOnlyTree.h"
#include "core/typeutil.h"
#include "parser/FilterSpec.h"

#include <compare>
#include <iosfwd>
#include <string>
#include <string_view>

namespace sq::parser {

inline constexpr std::string_view ast_root_node_name = "root";

enum class FieldAccessType { Default, Pullup };

std::ostream &operator<<(std::ostream &os, const FieldAccessType &access_type);

/**
 * The data for a node in an abstract syntax tree (AST) for an input query.
 *
 * Each node in the AST represents an access of field of a system object.
 *
 * E.g. a query like 'a.b("param1") { c[3] d }' will have an AST something like:
 *      NODE[name="a"; params=[]; filter=None; children=[
 *          NODE[name="b"; params=["param1"]; Filter=None; children=[
 *              NODE[name="c"; params=[]; filter=ElementAccess(3); children=[]],
 *              NODE[name="d"; params=[]; filter=None; children=[]]
 *          ]
 *      ]
 *
 * Note that this class just defines the data for each node. The tree structure
 * is provided by MoveOnlyTree.
 */
class AstData {
public:
  AstData() = delete;
  AstData(const AstData &) = delete;
  AstData &operator=(const AstData &) = delete;

  AstData(AstData &&) = default;
  AstData &operator=(AstData &&) = default;
  ~AstData() noexcept = default;

  AstData(std::string_view name, FieldAccessType access_type)
      : name_{name}, access_type_{access_type} {}

  /**
   * Name of the field being accessed.
   */
  SQ_ND const std::string &name() const { return name_; }

  /**
   * The type of the field access.
   */
  SQ_ND FieldAccessType access_type() const { return access_type_; }

  ///@{
  /**
   * Params to pass to the system when accessing the field.
   */
  SQ_ND const FieldCallParams &params() const { return params_; }
  SQ_ND FieldCallParams &params() { return params_; }
  ///@}

  ///@{
  /**
   * Details of the filter specified for the results of accessing the field.
   */
  SQ_ND const FilterSpec &filter_spec() const { return filter_spec_; }
  SQ_ND FilterSpec &filter_spec() { return filter_spec_; }
  ///@}

  SQ_ND auto operator<=>(const AstData &) const = default;

private:
  std::string name_;
  FieldAccessType access_type_;
  FieldCallParams params_;
  FilterSpec filter_spec_;
};
std::ostream &operator<<(std::ostream &os, const AstData &ast_data);

using Ast = MoveOnlyTree<AstData>;

} // namespace sq::parser

#endif // SQ_INCLUDE_GUARD_parser_Ast_h_
