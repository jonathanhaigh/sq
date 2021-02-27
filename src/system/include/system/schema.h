/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_schema_h_
#define SQ_INCLUDE_GUARD_system_schema_h_

#include <cstddef>
#include <gsl/gsl>
#include <string_view>

namespace sq::system {

class SchemaImpl;

class PrimitiveTypeSchema
{
public:
    constexpr explicit PrimitiveTypeSchema(std::string_view name)
        : name_{name}
    { }

    [[nodiscard]] std::string_view name() const;

private:
    std::string_view name_;
};

class ParamSchema
{
public:
    constexpr ParamSchema(
        std::string_view name,
        std::size_t index,
        std::size_t type_index
    )
        : name_{name}
        , index_{index}
        , type_index_{type_index}
    { }

    [[nodiscard]] std::string_view name() const;
    [[nodiscard]] std::size_t index() const;
    [[nodiscard]] const PrimitiveTypeSchema& type() const;

private:
    std::string_view name_;
    std::size_t index_;
    std::size_t type_index_;
};

class TypeSchema;

class FieldSchema
{
public:
    constexpr FieldSchema(
        std::string_view name,
        std::size_t params_begin_index,
        std::size_t params_end_index,
        std::size_t return_type_index,
        bool return_list
    )
        : name_{name}
        , params_begin_index_{params_begin_index}
        , params_end_index_{params_end_index}
        , return_type_index_{return_type_index}
        , return_list_{return_list}
    { }

    [[nodiscard]] std::string_view name() const;
    [[nodiscard]] gsl::span<const ParamSchema> params() const;
    [[nodiscard]] const TypeSchema& return_type() const;
    [[nodiscard]] bool return_list() const;

private:
    std::string_view name_;
    std::size_t params_begin_index_;
    std::size_t params_end_index_;
    std::size_t return_type_index_;
    bool return_list_;
};

class TypeSchema
{
public:
    constexpr TypeSchema(
        std::string_view name,
        std::size_t fields_begin_index,
        std::size_t fields_end_index
    )
        : name_{name}
        , fields_begin_index_{fields_begin_index}
        , fields_end_index_{fields_end_index}
    { }

    [[nodiscard]] std::string_view name() const;
    [[nodiscard]] gsl::span<const FieldSchema> fields() const;

private:
    std::string_view name_;
    std::size_t fields_begin_index_;
    std::size_t fields_end_index_;
};

struct Schema
{
public:
    [[nodiscard]] gsl::span<const TypeSchema> types() const;
    [[nodiscard]] gsl::span<const PrimitiveTypeSchema> primitive_types() const;
    [[nodiscard]] const TypeSchema& root_type() const;
};

const Schema& schema();

} // namespace sq::system

#endif // SQ_INCLUDE_GUARD_system_schema_h_
