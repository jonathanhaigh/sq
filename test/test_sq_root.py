# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import pytest
import util

simple_tests = []
out_of_range_tests = []
invalid_args_tests = []

# SqRoot::int
simple_tests.extend((f"int({i})",       { "int": int(i) }) for i in util.INT_STRS)
simple_tests.extend((f"int(value={i})", { "int": int(i) }) for i in util.INT_STRS)
out_of_range_tests.extend(f"int({i})" for i in util.OUT_OF_RANGE_INT_STRS)
invalid_args_tests.extend(f"int({i})" for i in ("1.0", "true", "false", '"str"'))

# SqRoot::float
simple_tests.extend((f"float({i})",       { "float": float(i) }) for i in util.FLOAT_STRS)
simple_tests.extend((f"float(value={i})", { "float": float(i) }) for i in util.FLOAT_STRS)
simple_tests.extend((f"float({i})",       { "float": float(i) }) for i in ("-1", "1", "0"))
out_of_range_tests.extend(f"float({i})" for i in util.OUT_OF_RANGE_FLOAT_STRS)
invalid_args_tests.extend(f"float({i})" for i in ("true", "false", '"str"'))

# SqRoot::bool
simple_tests.append(("bool(true)",        { "bool": True }))
simple_tests.append(("bool(false)",       { "bool": False }))
simple_tests.append(("bool(value=true)",  { "bool": True }))
simple_tests.append(("bool(value=false)", { "bool": False }))
invalid_args_tests.extend(f"bool({i})" for i in ('"true"', '"false"', "1", "1.0" ))

# SqRoot::string
simple_tests.extend((f'string({util.quote(i)})',       { "string": i }) for i in util.STRINGS)
simple_tests.extend((f'string(value={util.quote(i)})', { "string": i }) for i in util.STRINGS)
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
simple_tests.extend((f'path({util.quote(i)})', { "path": i }) for i in util.PATH_STRS)

@pytest.mark.parametrize("query,result", simple_tests)
def test_simple(query, result):
    assert util.sq(query) == result

@pytest.mark.parametrize("query", out_of_range_tests)
def test_out_of_range(query):
    util.sq_error(query, "out ?of ?range")

@pytest.mark.parametrize("query", invalid_args_tests)
def test_invalid_argument(query):
    util.sq_error(query, "invalid ?argument")
