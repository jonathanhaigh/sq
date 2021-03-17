# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import copy
import pytest
import util


def flatten_doc_list(entity):
    if isinstance(entity["doc"], list):
        entity["doc"] = "\n".join(entity["doc"])


def test_schema(sq_schema):
    # We're going to test that the schema returned by SQ is (mostly) the same
    # as the original schema in schema.json.
    #
    # There are a couple of things that will be different between the two
    # schemas though:
    # * doc arrays will have been converted to single strings with newlines.
    # * optional fields will always exist but might be null.
    #
    # Modify the schema we got from schema.json to match what we think SQ
    # should return, then just do a test using "=="
    schema = copy.deepcopy(sq_schema)
    for t in schema["types"]:
        flatten_doc_list(t)
        for f in t["fields"]:
            flatten_doc_list(f)
            for p in f["params"]:
                flatten_doc_list(p)
                if "default_value" not in p:
                    p["default_value"] = None
                if "default_value_doc" not in p:
                    p["default_value_doc"] = None

    result = util.sq(
        "schema {"
            "types {"
                "name doc fields { "
                    "name doc return_type return_list null params {"
                        "name doc index type required "
                        "default_value default_value_doc"
                    "}"
                "}"
            "}"
            " primitive_types { name doc }"
            " root_type"
        "}"
    )
    assert result == {"schema": schema}


def test_schema_to_primitive():
    assert util.sq('<schema.<types[="SqRoot"]') == ["SqRoot"]
    assert util.sq('<schema.<primitive_types[="PrimitiveInt"]') == [
        "PrimitiveInt"
    ]
    assert util.sq('<schema.<types[="SqRoot"].<fields[="schema"]') == [
        ["schema"]
    ]
    assert util.sq(
        '<schema.<types[="SqRoot"].<fields[="int"].<params[="value"]'
    ) == [[["value"]]]
