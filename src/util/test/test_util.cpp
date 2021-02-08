#include "util/strutil.h"

#include <gtest/gtest.h>
#include <initializer_list>

namespace sq::util::test {

TEST(UtilTest, JoinCStrings)
{
   const auto ints = {"0", "1", "2", "3", "4"};
   std::ostringstream ss;
   ss << join(ints);
   EXPECT_EQ(ss.str(), "0, 1, 2, 3, 4");

}

TEST(UtilTest, JoinCStringsWithDelim)
{
   const auto ints = {"0", "1", "2", "3", "4"};
   std::ostringstream ss;
   ss << join(ints, "*");
   EXPECT_EQ(ss.str(), "0*1*2*3*4");

}

TEST(UtilTest, JoinInts)
{
   const auto ints = {0, 1, 2, 3, 4};
   std::ostringstream ss;
   ss << join(ints);
   EXPECT_EQ(ss.str(), "0, 1, 2, 3, 4");
}

TEST(UtilTest, JoinIntsWithDelim)
{
   const auto ints = {0, 1, 2, 3, 4};
   std::ostringstream ss;
   ss << join(ints, "()");
   EXPECT_EQ(ss.str(), "0()1()2()3()4");
}

TEST(UtilTest, JoinIntsWithEmptyDelim)
{
   const auto ints = {0, 1, 2, 3, 4};
   std::ostringstream ss;
   ss << join(ints, "");
   EXPECT_EQ(ss.str(), "01234");
}

TEST(UtilTest, Join0)
{
   const auto ints = std::initializer_list<int>{};
   std::ostringstream ss;
   ss << join(ints);
   EXPECT_EQ(ss.str(), "");

}

TEST(UtilTest, Join0WithDelim)
{
   const auto ints = std::initializer_list<int>{};
   std::ostringstream ss;
   ss << join(ints, "*");
   EXPECT_EQ(ss.str(), "");

}

TEST(UtilTest, Join1)
{
   const auto ints = { 1 };
   std::ostringstream ss;
   ss << join(ints);
   EXPECT_EQ(ss.str(), "1");
}

TEST(UtilTest, Join1WithDelim)
{
   const auto ints = { 1 };
   std::ostringstream ss;
   ss << join(ints, "*");
   EXPECT_EQ(ss.str(), "1");
}

TEST(UtilTest, VariantToStr)
{
    const auto ints = { 0, 1, 2, 3, 4 };
    std::ostringstream ss;
    for (auto i : ints)
    {
        auto v = std::variant<int, std::string>{i};
        ss << variant_to_str(v);
        v = "*";
        ss << variant_to_str(v);
    }
    EXPECT_EQ(ss.str(), "0*1*2*3*4*");
}

TEST(UtilTest, OptionalToStr)
{
    const auto ints = { 0, 1, 2, 3, 4 };
    std::ostringstream ss;
    for (auto i : ints)
    {
        std::optional<int> opt;
        if (i % 2)
        {
            opt = i;
        }
        ss << optional_to_str(opt);
    }
    EXPECT_EQ(ss.str(), "13");
}

} // namespace sq::util::test
