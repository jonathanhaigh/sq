# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import pathlib
import pwd
import pytest
import util

def test_uid(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = tmp_path.stat().st_uid
    result =  util.sq(f"<path({quoted_path}).<file.<user.<uid")
    assert result == expected

def test_username(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = pwd.getpwuid(tmp_path.stat().st_uid).pw_name
    result =  util.sq(f"<path({quoted_path}).<file.<user.<username")
    assert result == expected

def test_group(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = pwd.getpwuid(tmp_path.stat().st_uid).pw_gid
    result =  util.sq(f"<path({quoted_path}).<file.<user.<group")
    assert result == expected

def test_gecos(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = pwd.getpwuid(tmp_path.stat().st_uid).pw_gecos.split(',')
    result =  util.sq(f"<path({quoted_path}).<file.<user.<gecos")
    assert result == expected

def test_name(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = pwd.getpwuid(tmp_path.stat().st_uid).pw_gecos.split(',')[0]
    result =  util.sq(f"<path({quoted_path}).<file.<user.<name")
    assert result == expected

def test_home(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = pwd.getpwuid(tmp_path.stat().st_uid).pw_dir
    result =  util.sq(f"<path({quoted_path}).<file.<user.<home")
    assert result == expected

def test_shell(tmp_path):
    quoted_path = util.quote(str(tmp_path))
    expected = pwd.getpwuid(tmp_path.stat().st_uid).pw_shell
    result =  util.sq(f"<path({quoted_path}).<file.<user.<shell")
    assert result == expected
