# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

include(FetchContent)

# This fetches the latest commit on master as of 2021/03/13. The latest
# released version (7.1.3) doesn't include a fix for
# https://github.com/fmtlib/fmt/issues/2040, which we need. When there's a
# release containing that fix then maybe we can switch to it.
FetchContent_Declare(get_fmt
    URL https://github.com/fmtlib/fmt/archive/b8ff3c182004571dd9e1452239f1da6fde8f23e0.tar.gz
    DOWNLOAD_NO_PROGRESS TRUE
)
FetchContent_MakeAvailable(get_fmt)
