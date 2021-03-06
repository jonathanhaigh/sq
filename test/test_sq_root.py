#!/usr/bin/env python3
# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import pytest
import util

INT_STRS = (
    "0", "-1", "1",
    "-9223372036854775808", "9223372036854775807",
)
OUT_OF_RANGE_INT_STRS = (
    -9223372036854775809, 9223372036854775808,
)

FLOAT_STRS = (
    "0.0", ".0", "-0.0", "+0.0",
    "1.0", "-1.0", "+1.0",
    "1.0e-307", "1.0e+308", "-1.0e+308",
)
OUT_OF_RANGE_FLOAT_STRS = (
    "1.0e-309", "1.0e+309", "-1.0e+309",
)

STRINGS = (
    "str", "str with spaces",
    "0", "1", "-1",
    "0.0", "1.0", "-1.0",
    "true", "false",
)

PATH_STRS = (
    ".", "./", "..", "../",
    "/", "/usr", "/usr/bin", "/usr/bin/",
    "usr", "usr/share", "usr/share/file.txt",
    "/home/user01/file.tar.gz",
    "C:", "C:\\", "C:\\program files\\file.zip",
    "///share/dir/.abc", "öξちシДน",
    "|\t\n\r<>{}()[]'\"#£$~@:;`!%&^*-+_=",
)

simple_tests = []
out_of_range_tests = []
invalid_args_tests = []

# SqRoot::int
simple_tests.extend((f"int({i})",       { "int": int(i) }) for i in INT_STRS)
simple_tests.extend((f"int(value={i})", { "int": int(i) }) for i in INT_STRS)
out_of_range_tests.extend(f"int({i})" for i in OUT_OF_RANGE_INT_STRS)
invalid_args_tests.extend(f"int({i})" for i in ("1.0", "true", "false", '"str"'))

# SqRoot::float
simple_tests.extend((f"float({i})",       { "float": float(i) }) for i in FLOAT_STRS)
simple_tests.extend((f"float(value={i})", { "float": float(i) }) for i in FLOAT_STRS)
simple_tests.extend((f"float({i})",       { "float": float(i) }) for i in ("-1", "1", "0"))
out_of_range_tests.extend(f"float({i})" for i in OUT_OF_RANGE_FLOAT_STRS)
invalid_args_tests.extend(f"float({i})" for i in ("true", "false", '"str"'))

# SqRoot::bool
simple_tests.append(("bool(true)",        { "bool": True }))
simple_tests.append(("bool(false)",       { "bool": False }))
simple_tests.append(("bool(value=true)",  { "bool": True }))
simple_tests.append(("bool(value=false)", { "bool": False }))
invalid_args_tests.extend(f"bool({i})" for i in ('"true"', '"false"', "1", "1.0" ))

# SqRoot::string
simple_tests.extend((f'string("{i}")',       { "string": i }) for i in STRINGS)
simple_tests.extend((f'string(value="{i}")', { "string": i }) for i in STRINGS)
invalid_args_tests.extend(f"string({i})" for i in ("-1" "1.0", "true", "false"))

# SqRoot::ints
simple_tests.extend([
    ("ints(0, 5)",            { "ints": [ 0,  1,  2,  3,  4] }),
    ("ints(-5, 0)",           { "ints": [-5, -4, -3, -2, -1] }),
    ("ints(-6, -1)",          { "ints": [-6, -5, -4, -3, -2] }),
    ("ints(5)[:5]",           { "ints": [ 5,  6,  7,  8,  9] }),
    ("ints(-5)[:5]",          { "ints": [-5, -4, -3, -2, -1] }),
    ("ints(start=5)[:5]",     { "ints": [ 5,  6,  7,  8,  9] }),
    ("ints(stop=5)",          { "ints": [ 0,  1,  2,  3,  4] }),
    ("ints(5, stop=10)",      { "ints": [ 5,  6,  7,  8,  9] }),
    ("ints(start=5, stop=10)",{ "ints": [ 5,  6,  7,  8,  9] }),
])

# SqRoot::path
simple_tests.extend((f'path("{i}")', { "path": i }) for i in PATH_STRS)

@pytest.mark.parametrize("query,result", simple_tests)
def test_simple(query, result):
    assert util.sq(query) == result

@pytest.mark.parametrize("query", out_of_range_tests)
def test_out_of_range(query):
    util.sq_error(query, "out ?of ?range")

@pytest.mark.parametrize("query", invalid_args_tests)
def test_invalid_argument(query):
    util.sq_error(query, "invalid ?argument")
