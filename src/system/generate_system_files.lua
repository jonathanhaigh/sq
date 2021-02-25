#!/bin/env lua
-- -----------------------------------------------------------------------------
-- Copyright 2021 Jonathan Haigh
-- SPDX-License-Identifier: MIT
-- -----------------------------------------------------------------------------

schema_path = arg[1]
template_path = arg[2]
output_path_format = arg[3]

function sq_param(param_index, param_schema)
    local ptr_or_ref = param_schema["required"] and "&" or "*"
    local param_type = "const " .. param_schema["type"] .. ptr_or_ref
    return {
        name = param_schema["name"],
        index = param_index,
        base_type = param_schema["type"],
        required = param_schema["required"],
        type = param_type,
    }
end

function sq_field(field_name, field_schema)
    local field_params = {}
    for index, param_schema in pairs(field_schema["params"]) do
        table.insert(field_params, sq_param(index - 1, param_schema))
    end
    return {
        name = field_name,
        return_type = field_schema["return"],
        params = field_params,
    }
end

function sq_type(type_name, type_schema)
    local field_tables = {}
    for field_name, field_schema in pairs(type_schema) do
        table.insert(field_tables, sq_field(field_name, field_schema))
    end
    return {
        name = type_name,
        fields = field_tables,
    }
end

function get_schema()
    local lunajson = require("lunajson")
    local f = assert(io.open(schema_path, "rb"))
    local schema = lunajson.decode(f:read("*all"))
    f:close()
    return schema
end

function compile_template(liluat, path)
    local f = assert(io.open(path, "rb"))
    local template = liluat.compile(f:read("*all"))
    f:close()
    return template
end

function generate_system_files()
    local liluat = require("liluat")
    local template = compile_template(liluat, template_path)
    for type_name, type_schema in pairs(get_schema()) do
        local rendered = liluat.render(template, sq_type(type_name, type_schema))
        local output_path = string.format(output_path_format, type_name)
        local f = assert(io.open(output_path, "wb"))
        f:write(rendered)
        f:close()
    end
end

generate_system_files()
