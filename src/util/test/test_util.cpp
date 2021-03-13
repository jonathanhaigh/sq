/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "util/MoveOnlyTree.h"
#include "util/strutil.h"

#include <gtest/gtest.h>
#include <initializer_list>

namespace sq::test {

using namespace sq::util;

TEST(UtilTest, VariantFmtSpecialization) {
  using Variant = std::variant<int, std::string>;

  const auto v1 = Variant{10};
  EXPECT_EQ(fmt::to_string(v1), std::string{"10"});

  const auto v2 = Variant{std::string{"a string"}};
  EXPECT_EQ(fmt::to_string(v2), std::string{"a string"});
}

TEST(UtilTest, OptionalFmtSpecialization) {
  using Optional = std::optional<int>;

  const auto o1 = Optional{10};
  EXPECT_EQ(fmt::to_string(o1), std::string{"10"});

  const auto o2 = Optional{};
  EXPECT_EQ(fmt::to_string(o2), std::string{});
}

TEST(UtilTest, MoveOnlyTree) {
  using IL = std::initializer_list<int>;
  auto mkchildren = [](auto child_vals) {
    return std::vector<MoveOnlyTree<int>>(std::begin(child_vals),
                                          std::end(child_vals));
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

} // namespace sq::test
