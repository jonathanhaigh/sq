#!/bin/env lua
-- -----------------------------------------------------------------------------
-- Copyright 2021 Jonathan Haigh
-- SPDX-License-Identifier: MIT
-- -----------------------------------------------------------------------------

local sq_schema_util = { }

local function load_schema(path)
    local lunajson = require("lunajson")
    local f = assert(io.open(path, "rb"))
    local schema = lunajson.decode(f:read("*all"))
    f:close()
    return schema
end

local function compile_template(liluat, path)
    local f = assert(io.open(path, "rb"))
    local template = liluat.compile(f:read("*all"))
    f:close()
    return template
end

local function render_template_to_file(liluat, template, output_path, data)
    local rendered = liluat.render(template, data)
    local f = assert(io.open(output_path, "wb"))
    f:write(rendered)
    f:close()
end

local function render_template_for_type(liluat, template, output_path_format, type_schema)
    local output_path = string.format(output_path_format, type_schema.name)
    render_template_to_file(liluat, template, output_path, type_schema)
end

local function render_template_for_each_type(liluat, template, output_path_format, schema)
    for _, type_schema in ipairs(schema.types) do
        render_template_for_type(liluat, template, output_path_format, type_schema)
    end
end

function sq_schema_util.generate_file_for_each_type(schema_path, template_path, output_path_format)
    local liluat = require("liluat")
    local template = compile_template(liluat, template_path)
    local schema = load_schema(schema_path)
    render_template_for_each_type(liluat, template, output_path_format, schema)
end

function sq_schema_util.generate_file(schema_path, template_path, output_path)
    local liluat = require("liluat")
    local template = compile_template(liluat, template_path)
    local schema = load_schema(schema_path)
    render_template_to_file(liluat, template, output_path, schema)
end

return sq_schema_util
