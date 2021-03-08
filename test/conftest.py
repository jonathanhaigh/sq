# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import json
import os
import pathlib
import pytest

DEFAULT_SCHEMA = pathlib.Path(__file__).parent.parent / "src/system/schema.json"

def pytest_addoption(parser):
    parser.addoption("--schema", action="store", default=DEFAULT_SCHEMA)

def pytest_generate_tests(metafunc):
    # This is called for every test. Only get/set command line arguments
    # if the argument is specified in the list of test "fixturenames".
    schema_path = metafunc.config.option.schema
    schema = None
    with open(schema_path) as f:
        schema = json.load(f)
    if "sq_schema" in metafunc.fixturenames:
        metafunc.parametrize("sq_schema", [schema])
    if "sq_primitive_type" in metafunc.fixturenames:
        metafunc.parametrize("sq_primitive_type", schema["primitive_types"])
    if "sq_type" in metafunc.fixturenames:
        metafunc.parametrize("sq_type", schema["types"])
