# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import pathlib
import pytest
import util


@pytest.mark.parametrize("path", util.PATH_STRS)
def test_string(path):
    assert util.sq(f"path({util.quote(path)}).string") == {
        "path": {"string": path}
    }


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
        assert util.sq(f"path({util.quote(path_info['path'])}).{attr}") == {
            "path": {attr: path_info[attr]}
        }


@pytest.mark.parametrize("path_info", all_path_infos)
def test_absolute(path_info):
    path = path_info["path"]

    absolute_path = path_info["path"]
    if not path_info["is_absolute"]:
        absolute_path = f"{pathlib.Path.cwd()}/{path}"

    assert util.sq(f"path({util.quote(path)}).absolute") == {
        "path": {"absolute": absolute_path}
    }


@pytest.mark.parametrize("path_info", relative_path_infos)
def test_canonical(tmp_path, path_info):
    path = path_info["path"]
    canonical_path = (tmp_path / path).resolve()
    canonical_path.parent.mkdir(parents=True, exist_ok=True)
    canonical_path.touch(exist_ok=True)
    assert util.sq(f"path({util.quote(path)}).canonical", cwd=tmp_path) == {
        "path": {"canonical": str(canonical_path)}
    }


def test_size(tmp_path):
    path = tmp_path / "file"
    path.write_text("Some data")
    size = path.stat().st_size
    assert util.sq(f"path({util.quote(str(path))}).size") == {
        "path": {"size": size}
    }


def test_children(tmp_path):
    children = [tmp_path / f for f in ("f1", "x", "achild")]
    for child in children:
        child.touch()
    assert sorted(
        util.sq(f"path.children", cwd=tmp_path)["path"]["children"]
    ) == sorted([str(child) for child in children])
