#!/bin/env lua
-- -----------------------------------------------------------------------------
-- Copyright 2021 Jonathan Haigh
-- SPDX-License-Identifier: MIT
-- -----------------------------------------------------------------------------

schema_path = arg[1]
template_path = arg[2]
output_path_format = arg[3]

local ssu = require("sq_schema_util")
ssu.generate_file_for_each_type(schema_path, template_path, output_path_format)
