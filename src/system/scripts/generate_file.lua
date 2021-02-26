#!/bin/env lua
-- -----------------------------------------------------------------------------
-- Copyright 2021 Jonathan Haigh
-- SPDX-License-Identifier: MIT
-- -----------------------------------------------------------------------------

schema_path = arg[1]
template_path = arg[2]
output_path = arg[3]

local ssu = require("sq_schema_util")
ssu.generate_file(schema_path, template_path, output_path)
