#include "util/MoveOnlyTree.h"
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


TEST(UtilTest, MoveOnlyTree)
{
    using IL = std::initializer_list<int>;
    auto mkchildren = [](auto child_vals) {
        return std::vector<MoveOnlyTree<int>>(
            std::begin(child_vals),
            std::end(child_vals)
        );
    };
    auto mktree = [&mkchildren](int val, auto child_vals) {
        auto t = MoveOnlyTree<int>{val};
        t.children() = mkchildren(child_vals);
        return t;
    };
    auto no_children = IL{};

    auto tree = MoveOnlyTree<int>{0};
    EXPECT_EQ(tree, mktree(0, no_children));
    EXPECT_EQ(tree.data(), 0);
    EXPECT_EQ(tree.children(), mkchildren(no_children));


    tree.children().emplace_back(1);
    tree.children().emplace_back(2);
    tree.children().emplace_back(3);
    EXPECT_EQ(tree, mktree(0, IL{1, 2, 3}));
    EXPECT_EQ(tree.children(), mkchildren(IL{1, 2, 3}));
    EXPECT_EQ(tree.children()[0].data(), 1);
    EXPECT_TRUE(tree.children()[0].children().empty());
    EXPECT_EQ(tree.children()[1].data(), 2);
    EXPECT_TRUE(tree.children()[1].children().empty());
    EXPECT_EQ(tree.children()[2].data(), 3);
    EXPECT_TRUE(tree.children()[2].children().empty());
}

} // namespace sq::util::test
