# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import grp
import pathlib
import pytest
import util

def test_gid(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = tmp_path.stat().st_gid
    result =  util.sq(f"<path({quoted_path}).<file.<group.<gid")
    assert result == expected

def test_name(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = grp.getgrgid(tmp_path.stat().st_gid).gr_name
    result =  util.sq(f"<path({quoted_path}).<file.<group.<name")
    assert result == expected

def test_members(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = grp.getgrgid(tmp_path.stat().st_gid).gr_mem
    result = util.sq(f"<path({quoted_path}).<file.<group.<members")
    assert result == expected
