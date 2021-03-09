# ------------------------------------------------------------------------------
# Copyright 2021 Jonathan Haigh
# SPDX-License-Identifier: MIT
# ------------------------------------------------------------------------------

import itertools
import math
import pytest
import util

DATA_SIZES = itertools.chain(
    (0, 500, 512),
    (int(1.5 * (1000 ** x)) for x in (1, 2, 3, 4, 5, 6)),
    (int(2.5 * (1024 ** x)) for x in (1, 2, 3, 4, 5, 6)),
)

DECIMAL_UNITS = ("B", "kB", "MB", "GB", "TB", "PB", "EB")
BINARY_UNITS = ("B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB")


@pytest.mark.parametrize(
    "exponent,unit,base,data_size",
    (
        (exponent, unit, base, data_size)
        for data_size in DATA_SIZES
        for base, units_tuple in ((1000, DECIMAL_UNITS), (1024, BINARY_UNITS))
        for exponent, unit in enumerate(units_tuple)
    )
)
def test_data_size_units(exponent, unit, base, data_size):
    size_in_units = data_size / (base ** exponent)
    result = util.sq(f"data_size({data_size}).{unit}")["data_size"][unit]
    assert math.isclose(result, size_in_units)
