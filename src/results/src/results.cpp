/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/results.h"

#include "core/errors.h"
#include "core/typeutil.h"
#include "parser/Ast.h"
#include "results/Filter.h"
#include "results/Serializer.h"

namespace sq::results {

namespace {

bool is_pullup_node(const parser::Ast &ast_node) {
  return ast_node.data().access_type() == parser::FieldAccessType::Pullup;
}

class ResultStreamer {
public:
  ResultStreamer(const parser::Ast &ast, Serializer &serializer)
      : ast_{&ast}, serializer_{&serializer} {}

  void operator()(const PrimitiveNull &null);
  void operator()(const FieldPtr &field);
  void operator()(ranges::cpp20::view auto &&rng);

private:
  const parser::Ast *ast_;
  Serializer *serializer_;
};

void ResultStreamer::operator()(const PrimitiveNull &null) {
  serializer_->write_value(null);
}

void ResultStreamer::operator()(const FieldPtr &field) {
  if (ast_->children().empty()) {
    serializer_->write_value(field->to_primitive());
    return;
  }

  const auto &children = ast_->children();
  const bool pullup = children.size() == 1 && is_pullup_node(children.front());

  if (!pullup) {
    serializer_->start_object();
  }

  for (const auto &child : ast_->children()) {

    const auto &field_name = child.data().name();
    if (!pullup) {
      serializer_->write_key(field_name);
    }

    const auto &params = child.data().params();
    const auto filter = Filter::create(child.data().filter_spec());
    auto visitor = ResultStreamer{child, *serializer_};
    auto child_results = (*filter)(field->get(field_name, params));
    std::visit(visitor, std::move(child_results));

    if (!pullup && is_pullup_node(child)) {
      throw PullupWithSiblingsError{fmt::format(
          "cannot use pullup access for field \"{}\": it has sibling fields",
          field_name)};
    }
  }

  if (!pullup) {
    serializer_->end_object();
  }
}

void ResultStreamer::operator()(ranges::cpp20::view auto &&rng) {
  serializer_->start_array();
  for (auto field : SQ_FWD(rng)) {
    (*this)(field);
  }
  serializer_->end_array();
}

} // namespace

void generate_results(const parser::Ast &ast, const FieldPtr &system_root,
                      Serializer &serializer) {
  ResultStreamer{ast, serializer}(system_root);
}

} // namespace sq::results
