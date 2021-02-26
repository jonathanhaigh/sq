/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_schema_h_
#define SQ_INCLUDE_GUARD_system_schema_h_

#include <cstddef>
#include <span>
#include <string_view>

namespace sq::system {

struct PrimitiveTypeSchema
{
    std::string_view name;
};

struct ParamSchema
{
    std::string_view name;
    std::ptrdiff_t index;
    const PrimitiveTypeSchema* type;
};

struct TypeSchema;

struct FieldSchema
{
    std::string_view name;
    std::span<const ParamSchema> params;
    const TypeSchema* return_type;
    bool return_list;
};

struct TypeSchema
{
    std::string_view name;
    std::span<const FieldSchema> fields;
};

struct Schema
{
    std::span<const TypeSchema> types;
    std::span<const PrimitiveTypeSchema> primitive_types;
    const TypeSchema* root_type;
};

const Schema& schema();

} // namespace sq::system

#endif // SQ_INCLUDE_GUARD_system_schema_h_
