/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "serialization/serialize.h"

#include "results/results.h"
#include "test/results_test_util.h"

#include <cctype>
#include <gtest/gtest.h>
#include <range/v3/action/remove_if.hpp>

namespace sq::test {

inline bool isspace(char c)
{
    return std::isspace(static_cast<unsigned char>(c));
}

inline void test_serialization(std::string_view expected, ResultTree&& tree)
{
    SCOPED_TRACE(testing::Message()
        << "test_serialization("
        << "expected=" << expected << ", "
        << "tree)"
    );
    auto ss = std::ostringstream{};
    serialization::serialize_results(ss, tree);

    auto serialized = ss.str();
    serialized |= ranges::actions::remove_if(isspace);

    EXPECT_EQ(serialized, expected);
}

TEST(SerializationTest, TestSerialization)
{
    test_serialization(
        "{\"a\":1}",
        obj_data_tree("a", 1)
    );
    test_serialization(
        "{\"a\":-1}",
        obj_data_tree("a", -1)
    );
    test_serialization(
        "{\"a\":99}",
        obj_data_tree("a", 99)
    );
    test_serialization(
        "{\"a\":-99}",
        obj_data_tree("a", -99)
    );
    test_serialization(
        "{\"a\":1.2}",
        obj_data_tree("a", 1.2)
    );
    test_serialization(
        "{\"a\":-1.2}",
        obj_data_tree("a", -1.2)
    );
    test_serialization(
        "{\"a\":99.2}",
        obj_data_tree("a", 99.2)
    );
    test_serialization(
        "{\"a\":-99.2}",
        obj_data_tree("a", -99.2)
    );
    test_serialization(
        "{\"a\":1.0}",
        obj_data_tree("a", 1.0)
    );
    test_serialization(
        "{\"a\":-1.0}",
        obj_data_tree("a", -1.0)
    );
    test_serialization(
        "{\"a\":99.0}",
        obj_data_tree("a", 99.0)
    );
    test_serialization(
        "{\"a\":-99.0}",
        obj_data_tree("a", -99.0)
    );
    test_serialization(
        "{\"a\":\"\"}",
        obj_data_tree("a", "")
    );
    test_serialization(
        "{\"a\":\"str\"}",
        obj_data_tree("a", "str")
    );
    test_serialization(
        "{\"a\":true}",
        obj_data_tree("a", true)
    );
    test_serialization(
        "{\"a\":false}",
        obj_data_tree("a", false)
    );
    test_serialization(
        "{\"a\":[1,2,3,4,5]}",
        obj_data_tree("a", array_data_tree(1, 2, 3, 4, 5))
    );
    test_serialization(
        "{\"a\":[\"1\",\"2\",\"3\",\"4\",\"5\"]}",
        obj_data_tree("a", array_data_tree("1", "2", "3", "4", "5"))
    );
    test_serialization(
        "{\"a\":[true,false]}",
        obj_data_tree("a", array_data_tree(true, false))
    );
    test_serialization(
        "{\"a\":[-1.0,0.0,1.0]}",
        obj_data_tree("a", array_data_tree(-1.0, 0.0, 1.0))
    );
    test_serialization(
        "{\"a\":{\"b\":1,\"c\":2}}",
        obj_data_tree("a", obj_data_tree("b", 1, "c", 2))
    );
}

} // namespace sq::test
