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

def log(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def sq_binary():
    if "SQ_BINARY" in os.environ:
        return os.environ["SQ_BINARY"]

    sq = shutil.which("sq")
    if sq is not None:
        return sq

    raise RuntimeError("Could not find SQ binary")

def sq(query):
    log(f"SQ query: {query}")
    return json.loads(subprocess.run(
        [sq_binary(), query],
        capture_output=True,
        check=True,
        text=True,
    ).stdout)

def sq_error(query, pattern = None, flags = re.I):
    with pytest.raises(subprocess.CalledProcessError) as e:
        sq(query)

    log(f"SQ stderr: {e.value.stderr}")
    log(f"SQ stdout: {e.value.stdout}")
    if pattern:
        assert re.match(pattern, e.value.stderr, flags)

def quote(string):
    return '"{}"'.format(string.replace("\\", "\\\\").replace("\"", "\\\""))
