import itertools
import os
import pathlib
import pytest
import stat
import util

FILE_TYPES = ("regular", "directory", "fifo", "symlink")

def create_file_of_type(path, file_type):
    if file_type == "regular":
        path.touch()
        path.write_text("Some data")
    elif file_type == "directory":
        path.mkdir()
    elif file_type == "fifo":
        os.mkfifo(path)
    elif file_type == "symlink":
        path.symlink_to("/some/symlink/target")

    if file_type in ("regular", "symlink"):
        return path.lstat().st_size
    return None

@pytest.mark.parametrize("file_type", FILE_TYPES)
def test_size(tmp_path, file_type):
    path = tmp_path / "file"
    expected = create_file_of_type(path, file_type)
    quoted_path = util.quote(str(path))
    query = f"<path({quoted_path}).<file(false).<size"
    assert util.sq(query) == expected

@pytest.mark.parametrize("file_type", FILE_TYPES)
def test_type(tmp_path, file_type):
    path = tmp_path / "file"
    create_file_of_type(path, file_type)
    quoted_path = util.quote(str(path))
    query = f"<path({quoted_path}).<file(false).<type"
    assert util.sq(query) == file_type

def test_hard_link_count(tmp_path):
    path = tmp_path / "file"
    quoted_path = util.quote(str(path))
    path.touch()
    assert util.sq(f"<path({quoted_path}).<file.<hard_link_count") == 1

    # Note: pathlib.Path.link_to() will be deprecated in Python 3.9 due to the
    # strange argument order (opposite to pathlib.Path.symlink_to()) but we
    # won't have the replacement, pathlib.Path.hardlink_to() until Python 3.9.
    # See https://bugs.python.org/issue39291 and
    #     https://bugs.python.org/issue39950
    path.link_to(tmp_path / "file2")
    assert util.sq(f"<path({quoted_path}).<file.<hard_link_count") == 2
    path.link_to(tmp_path / "file3")
    assert util.sq(f"<path({quoted_path}).<file.<hard_link_count") == 3

def test_mode(tmp_path):
    path = tmp_path / "file"
    path.touch()
    quoted_path = util.quote(str(path))
    assert util.sq(f"<path({quoted_path}).<file.<mode") == stat.S_IMODE(path.stat().st_mode)

def test_inode(tmp_path):
    path = tmp_path / "file"
    path.touch()
    quoted_path = util.quote(str(path))
    assert util.sq(f"<path({quoted_path}).<file.<inode") == path.stat().st_ino
