#!/usr/bin/env python3
# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import json
import os
import pytest
import re
import shutil
import subprocess
import sys

INT_STRS = (
    "0",
    "-1",
    "1",
    "-9223372036854775808",
    "9223372036854775807",
)
OUT_OF_RANGE_INT_STRS = (
    -9223372036854775809,
    9223372036854775808,
)

FLOAT_STRS = (
    "0.0",
    ".0",
    "-0.0",
    "+0.0",
    "1.0",
    "-1.0",
    "+1.0",
    "1.0e-307",
    "1.0e+308",
    "-1.0e+308",
)
OUT_OF_RANGE_FLOAT_STRS = (
    "1.0e-309",
    "1.0e+309",
    "-1.0e+309",
)

STRINGS = (
    "str",
    "str with spaces",
    "0",
    "1",
    "-1",
    "0.0",
    "1.0",
    "-1.0",
    "true",
    "false",
)

PATH_STRS = (
    ".",
    "./",
    "..",
    "../",
    "/",
    "/usr",
    "/usr/bin",
    "/usr/bin/",
    "usr",
    "usr/share",
    "usr/share/file.txt",
    "/home/user01/file.tar.gz",
    "/a /path with/ spaces.txt",
    "///share/dir/.abc",
    "öξちシДน",
    "|\t\n\r<>{}()[]'\"#£$~@:;`!%&^*-+_=",
)


def log(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def sq_binary():
    if "SQ_BINARY" in os.environ:
        return os.environ["SQ_BINARY"]

    sq = shutil.which("sq")
    if sq is not None:
        return sq

    raise RuntimeError("Could not find SQ binary")


def sq(query, **kwargs):
    log(f"SQ query: {query}")
    output = subprocess.run(
            [sq_binary(), query],
            capture_output=True,
            check=True,
            text=True,
            **kwargs,
        ).stdout
    log(f"SQ output: {output}")
    return json.loads(output)


def sq_error(query, pattern=None, flags=re.I):
    with pytest.raises(subprocess.CalledProcessError) as e:
        sq(query)

    log(f"SQ stderr: {e.value.stderr}")
    log(f"SQ stdout: {e.value.stdout}")
    if pattern:
        assert re.search(pattern, e.value.stderr, flags)


def quote(string):
    return '"{}"'.format(string.replace("\\", "\\\\").replace('"', '\\"'))
