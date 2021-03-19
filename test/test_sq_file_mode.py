# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import os
import pathlib
import pytest
import stat
import util

def test_permissions(tmp_path):
    path = tmp_path / "file"
    path.touch()
    quoted_path = util.quote(str(path))
    expected = path.stat().st_mode & (stat.S_IRWXU | stat.S_IRWXG | stat.S_IRWXO)
    assert util.sq(f"<path({quoted_path}).<file.<mode.<permissions") == expected

@pytest.mark.parametrize(
    "bit,name", (
        (stat.S_ISUID, "suid"),
        (stat.S_ISGID, "sgid"),
        (stat.S_ISVTX, "sticky"),
    )
)
def test_mode_bit(tmp_path, bit, name):
    path = tmp_path / "file"
    path.touch()
    quoted_path = util.quote(str(path))
    query = f"<path({quoted_path}).<file.<mode.<{name}"
    assert util.sq(query) == False
    path.chmod(stat.S_IMODE(path.stat().st_mode) | bit)
    assert util.sq(query) == True
