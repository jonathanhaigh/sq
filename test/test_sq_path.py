# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import itertools
import os
import pathlib
import pytest
import stat
import util

relative_path_infos = [
    {
        "path": ".",
        "parent": "",
        "filename": ".",
        "stem": ".",
        "extension": "",
        "parts": ["."],
        "is_absolute": False,
    },
    {
        "path": "..",
        "parent": "",
        "filename": "..",
        "stem": "..",
        "extension": "",
        "parts": [".."],
        "is_absolute": False,
    },
    {
        "path": "./x",
        "parent": ".",
        "filename": "x",
        "stem": "x",
        "extension": "",
        "parts": [".", "x"],
        "is_absolute": False,
    },
    {
        "path": "../x",
        "parent": "..",
        "filename": "x",
        "stem": "x",
        "extension": "",
        "parts": ["..", "x"],
        "is_absolute": False,
    },
    {
        "path": "usr",
        "parent": "",
        "filename": "usr",
        "stem": "usr",
        "extension": "",
        "parts": ["usr"],
        "is_absolute": False,
    },
    {
        "path": "usr/share",
        "parent": "usr",
        "filename": "share",
        "stem": "share",
        "extension": "",
        "parts": ["usr", "share"],
        "is_absolute": False,
    },
    {
        "path": "a /path with/ spaces.txt",
        "parent": "a /path with",
        "filename": " spaces.txt",
        "stem": " spaces",
        "extension": ".txt",
        "parts": ["a ", "path with", " spaces.txt"],
        "is_absolute": False,
    },
]

absolute_path_infos = [
    {
        "path": "/",
        "parent": "/",
        "filename": "",
        "stem": "",
        "extension": "",
        "parts": ["/"],
        "is_absolute": True,
    },
    {
        "path": "/a/./b",
        "parent": "/a/.",
        "filename": "b",
        "stem": "b",
        "extension": "",
        "parts": ["/", "a", ".", "b"],
        "is_absolute": True,
    },
    {
        "path": "/a/../b",
        "parent": "/a/..",
        "filename": "b",
        "stem": "b",
        "extension": "",
        "parts": ["/", "a", "..", "b"],
        "is_absolute": True,
    },
    {
        "path": "/home/user1/file.tar.gz",
        "parent": "/home/user1",
        "filename": "file.tar.gz",
        "stem": "file.tar",
        "extension": ".gz",
        "parts": ["/", "home", "user1", "file.tar.gz"],
        "is_absolute": True,
    },
    {
        "path": "/home/user1/.file",
        "parent": "/home/user1",
        "filename": ".file",
        "stem": ".file",
        "extension": "",
        "parts": ["/", "home", "user1", ".file"],
        "is_absolute": True,
    },
    {
        "path": "/usr/bin/sq",
        "parent": "/usr/bin",
        "filename": "sq",
        "stem": "sq",
        "extension": "",
        "parts": ["/", "usr", "bin", "sq"],
        "is_absolute": True,
    },
    {
        "path": "/usr/bin/",
        "parent": "/usr/bin",
        "filename": "",
        "stem": "",
        "extension": "",
        "parts": ["/", "usr", "bin", ""],
        "is_absolute": True,
    },
]

all_path_infos = relative_path_infos + absolute_path_infos


@pytest.mark.parametrize("path", util.PATH_STRS)
def test_string(path):
    result = util.sq(f"<path({util.quote(path)}).<string")
    assert result == path


@pytest.mark.parametrize("path_info", all_path_infos)
def test_path(path_info):
    for attr in (
        "parent",
        "filename",
        "stem",
        "extension",
        "parts",
        "is_absolute",
    ):
        quoted_path = util.quote(path_info["path"])
        result = util.sq(f"<path({quoted_path}).<{attr}")
        assert result == path_info[attr]


@pytest.mark.parametrize("path_info", all_path_infos)
def test_absolute(path_info):
    path = path_info["path"]
    quoted_path = util.quote(path)

    absolute_path = path_info["path"]
    if not path_info["is_absolute"]:
        absolute_path = f"{pathlib.Path.cwd()}/{path}"

    result = util.sq(f"<path({quoted_path}).<absolute")
    assert result == absolute_path


@pytest.mark.parametrize("path_info", relative_path_infos)
def test_canonical(tmp_path, path_info):
    path = path_info["path"]
    quoted_path = util.quote(path)
    canonical_path = (tmp_path / path).resolve()
    canonical_path.parent.mkdir(parents=True, exist_ok=True)
    canonical_path.touch(exist_ok=True)
    result = util.sq(f"<path({quoted_path}).<canonical", cwd=tmp_path)
    assert result == str(canonical_path)


@pytest.mark.parametrize(
    "symlink,follow_symlinks,file_type",
    itertools.product(
        (True, False),
        (True, False),
        ("regular", "directory", "fifo"),
    )
)
def test_size(tmp_path, symlink, follow_symlinks, file_type):
    path = tmp_path / "file"
    expected = None
    if file_type == "regular":
        path.touch()
        path.write_text("Some data")
        expected = path.stat().st_size
    elif file_type == "directory":
        path.mkdir()
    elif file_type == "fifo":
        os.mkfifo(path)

    if symlink:
        link = tmp_path / "link"
        link.symlink_to(path)
        path = link
        if not follow_symlinks:
            expected = link.lstat().st_size

    quoted_path = util.quote(str(path))
    follow_symlinks_param = "true" if follow_symlinks else "false"
    query = f"<path({quoted_path}).<size({follow_symlinks_param})"
    assert util.sq(query) == expected


def test_size_of_non_existent(tmp_path):
    path = tmp_path / "nonexistent"
    quoted_path = util.quote(str(path))
    util.sq_error(f"<path({quoted_path}).<size", "filesystem ?error")


def test_size_of_broken_symlink(tmp_path):
    path = tmp_path / "broken_symlink"
    path.symlink_to(tmp_path / "nonexistent")
    quoted_path = util.quote(str(path))
    util.sq_error(f"<path({quoted_path}).<size", "filesystem ?error")


def test_children(tmp_path):
    children = [tmp_path / f for f in ("f1", "x", "achild")]
    for child in children:
        child.touch()
    result = sorted(util.sq(f"<path.<children", cwd=tmp_path))
    expected = sorted([str(child) for child in children])
    assert result == expected

@pytest.mark.parametrize(
    "symlink,follow_symlinks,exists",
    itertools.product((True, False), repeat=3)
)
def test_exists(tmp_path, symlink, follow_symlinks, exists):
    path = tmp_path / "file"
    if exists:
        path.touch()
    if symlink:
        link = tmp_path / "link"
        link.symlink_to(path)
        path = link
    follow_symlinks_param = "true" if follow_symlinks else "false"
    quoted_path = util.quote(str(path))
    query = f"<path({quoted_path}).<exists({follow_symlinks_param})"
    assert util.sq(query) == (exists or (symlink and not follow_symlinks))


@pytest.mark.parametrize(
    "symlink,follow_symlinks,file_type",
    itertools.product(
        (True, False),
        (True, False),
        ("regular", "directory", "fifo")
    )
)
def test_type(tmp_path, symlink, follow_symlinks, file_type):
    path = tmp_path / "file"
    if file_type == "regular":
        path.touch()
    elif file_type == "directory":
        path.mkdir()
    elif file_type == "fifo":
        os.mkfifo(path)

    expected = file_type
    if symlink:
        link = tmp_path / "link"
        link.symlink_to(path)
        path = link
        expected = file_type if follow_symlinks else "symlink"

    follow_symlinks_param = "true" if follow_symlinks else "false"
    quoted_path = util.quote(str(path))
    query = f"<path({quoted_path}).<type({follow_symlinks_param})"
    assert util.sq(query) == expected


def test_type_of_nonexistent(tmp_path):
    path = tmp_path / "nonexistent"
    quoted_path = util.quote(str(path))
    util.sq_error(f"<path({quoted_path}).<type", "filesystem ?error")


def test_type_of_broken_symlink(tmp_path):
    path = tmp_path / "broken_symlink"
    path.symlink_to(tmp_path / "nonexistent")
    quoted_path = util.quote(str(path))
    util.sq_error(f"<path({quoted_path}).<type", "filesystem ?error")

def test_hard_link_count(tmp_path):
    path = tmp_path / "file"
    quoted_path = util.quote(str(path))
    path.touch()
    assert util.sq(f"<path({quoted_path}).<hard_link_count") == 1

    # Note: pathlib.Path.link_to() will be deprecated in Python 3.9 due to the
    # strange argument order (opposite to pathlib.Path.symlink_to()) but we
    # won't have the replacement, pathlib.Path.hardlink_to() until Python 3.9.
    # See https://bugs.python.org/issue39291 and
    #     https://bugs.python.org/issue39950
    path.link_to(tmp_path / "file2")
    assert util.sq(f"<path({quoted_path}).<hard_link_count") == 2
    path.link_to(tmp_path / "file3")
    assert util.sq(f"<path({quoted_path}).<hard_link_count") == 3

def test_hard_link_count_of_nonexistent(tmp_path):
    path = tmp_path / "file"
    quoted_path = util.quote(str(path))
    util.sq_error(f"<path({quoted_path}).<hard_link_count", "filesystem ?error")

def test_mode(tmp_path):
    path = tmp_path / "file"
    path.touch()
    quoted_path = util.quote(str(path))
    assert util.sq(f"<path({quoted_path}).<mode") == stat.S_IMODE(path.stat().st_mode)
