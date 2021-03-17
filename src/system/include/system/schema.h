/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_schema_h_
#define SQ_INCLUDE_GUARD_system_schema_h_

#include "common_types/Primitive.h"
#include "util/typeutil.h"

#include <cstddef>
#include <gsl/gsl>
#include <optional>
#include <string_view>

namespace sq::system {

class SchemaImpl;

/**
 * Represents the schema for a primitive type.
 */
class PrimitiveTypeSchema {
public:
  constexpr explicit PrimitiveTypeSchema(std::string_view name,
                                         std::string_view doc)
      : name_{name}, doc_{doc} {}

  SQ_ND std::string_view name() const;
  SQ_ND std::string_view doc() const;

private:
  std::string_view name_;
  std::string_view doc_;
};

/**
 * Represents the schema for a parameter of field of a system object.
 */
class ParamSchema {
public:
  constexpr ParamSchema(std::string_view name, std::string_view doc,
                        std::size_t index, std::size_t type_index,
                        bool required, std::string_view default_value_str,
                        std::string_view default_value_doc)
      : name_{name}, doc_{doc}, index_{index}, type_index_{type_index},
        required_{required}, default_value_str_{default_value_str},
        default_value_doc_{default_value_doc} {}

  SQ_ND std::string_view name() const;
  SQ_ND std::string_view doc() const;
  SQ_ND std::size_t index() const;
  SQ_ND const PrimitiveTypeSchema &type() const;
  SQ_ND bool required() const;
  SQ_ND std::optional<Primitive> default_value() const;
  SQ_ND std::string_view default_value_doc() const;

private:
  std::string_view name_;
  std::string_view doc_;
  std::size_t index_;
  std::size_t type_index_;
  bool required_;
  std::string_view default_value_str_;
  std::string_view default_value_doc_;
};

class TypeSchema;

/**
 * Represents the schema for a field of a system object.
 */
class FieldSchema {
public:
  constexpr FieldSchema(std::string_view name, std::string_view doc,
                        std::size_t params_begin_index,
                        std::size_t params_end_index,
                        std::size_t return_type_index, bool return_list,
                        bool null)
      : name_{name}, doc_{doc}, params_begin_index_{params_begin_index},
        params_end_index_{params_end_index},
        return_type_index_{return_type_index},
        return_list_{return_list}, null_{null} {}

  SQ_ND std::string_view name() const;
  SQ_ND std::string_view doc() const;
  SQ_ND gsl::span<const ParamSchema> params() const;
  SQ_ND const TypeSchema &return_type() const;
  SQ_ND bool return_list() const;
  SQ_ND bool null() const;

private:
  std::string_view name_;
  std::string_view doc_;
  std::size_t params_begin_index_;
  std::size_t params_end_index_;
  std::size_t return_type_index_;
  bool return_list_;
  bool null_;
};

/**
 * Represents the schema for a system object.
 */
class TypeSchema {
public:
  constexpr TypeSchema(std::string_view name, std::string_view doc,
                       std::size_t fields_begin_index,
                       std::size_t fields_end_index)
      : name_{name}, doc_{doc}, fields_begin_index_{fields_begin_index},
        fields_end_index_{fields_end_index} {}

  SQ_ND std::string_view name() const;
  SQ_ND std::string_view doc() const;
  SQ_ND gsl::span<const FieldSchema> fields() const;

private:
  std::string_view name_;
  std::string_view doc_;
  std::size_t fields_begin_index_;
  std::size_t fields_end_index_;
};

/**
 * Represents the whole SQ schema.
 */
struct Schema {
public:
  SQ_ND gsl::span<const TypeSchema> types() const;
  SQ_ND gsl::span<const PrimitiveTypeSchema> primitive_types() const;
  SQ_ND const TypeSchema &root_type() const;
};

/**
 * Get the whole SQ schema.
 */
const Schema &schema();

} // namespace sq::system

#endif // SQ_INCLUDE_GUARD_system_schema_h_
