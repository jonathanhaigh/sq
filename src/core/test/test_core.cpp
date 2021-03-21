/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "core/FieldCallParams.h"
#include "core/MoveOnlyTree.h"
#include "core/Primitive.h"
#include "core/errors.h"
#include "core/strutil.h"
#include "core/typeutil.h"

#include <gtest/gtest.h>
#include <initializer_list>

namespace sq::test {

namespace {

template <Alternative<Primitive> T> void test_primitive_type() {
  SCOPED_TRACE(testing::Message()
               << "test_primitive_type<" << primitive_type_name_v<T> << ">()");
  EXPECT_EQ(primitive_type_name(T{}), primitive_type_name_v<T>);
  EXPECT_EQ(primitive_type_name(Primitive{T{}}), primitive_type_name_v<T>);
}

} // namespace

TEST(CommonTypesTest, TestPrimitive) {
  test_primitive_type<PrimitiveString>();
  test_primitive_type<PrimitiveInt>();
  test_primitive_type<PrimitiveFloat>();
  test_primitive_type<PrimitiveBool>();
  test_primitive_type<PrimitiveNull>();
}

TEST(CommonTypesTest, TestPrimitiveToStr) {
  EXPECT_EQ(primitive_to_str(Primitive{PrimitiveString{"a string"}}),
            "\"a string\"");
  EXPECT_EQ(primitive_to_str(
                Primitive{PrimitiveString{"\\a string \\with backslashes\\"}}),
            "\"\\\\a string \\\\with backslashes\\\\\"");
  EXPECT_EQ(primitive_to_str(Primitive{PrimitiveInt{1234}}), "1234");
  EXPECT_EQ(primitive_to_str(Primitive{PrimitiveInt{-1234}}), "-1234");
  EXPECT_EQ(primitive_to_str(Primitive{PrimitiveFloat{1.1}}), "1.1");
  EXPECT_EQ(primitive_to_str(Primitive{PrimitiveFloat{-1.1}}), "-1.1");
  EXPECT_EQ(primitive_to_str(Primitive{PrimitiveBool{true}}), "true");
  EXPECT_EQ(primitive_to_str(Primitive{PrimitiveBool{false}}), "false");
  EXPECT_EQ(primitive_to_str(Primitive{primitive_null}), "null");
}

class FieldCallParamsTest : public testing::Test {
protected:
  void SetUp() override {
    fcp_.pos_params().emplace_back(PrimitiveString{"str1"});
    fcp_.pos_params().emplace_back(PrimitiveInt{1});
    fcp_.named_params().emplace("s2", PrimitiveString{"str2"});
    fcp_.named_params().emplace("i2", PrimitiveInt{2});
  }

  FieldCallParams fcp_;
};

TEST_F(FieldCallParamsTest, TestCorrectGet) {
  SCOPED_TRACE(testing::Message() << "fcp_= " << fcp_);

  EXPECT_EQ(fcp_.get<PrimitiveString>(0, "s1"), "str1");
  EXPECT_EQ(fcp_.get<PrimitiveInt>(1, "i1"), 1);
  EXPECT_EQ(fcp_.get<PrimitiveString>(2, "s2"), "str2");
  EXPECT_EQ(fcp_.get<PrimitiveString>(3, "s2"), "str2");
  EXPECT_EQ(fcp_.get<PrimitiveInt>(2, "i2"), 2);
  EXPECT_EQ(fcp_.get<PrimitiveInt>(3, "i2"), 2);
}

TEST_F(FieldCallParamsTest, TestCorrectGetOptional) {
  SCOPED_TRACE(testing::Message() << "fcp_= " << fcp_);

  const auto s1 = fcp_.get_optional<PrimitiveString>(0, "s1");
  ASSERT_NE(s1, std::nullopt);
  EXPECT_EQ(s1, "str1");

  const auto i1 = fcp_.get_optional<PrimitiveInt>(1, "i1");
  ASSERT_NE(i1, std::nullopt);
  EXPECT_EQ(i1, 1);

  const auto s2a = fcp_.get_optional<PrimitiveString>(2, "s2");
  ASSERT_NE(s2a, std::nullopt);
  EXPECT_EQ(s2a, "str2");

  const auto s2b = fcp_.get_optional<PrimitiveString>(3, "s2");
  ASSERT_NE(s2b, std::nullopt);
  EXPECT_EQ(s2b, "str2");

  const auto i2a = fcp_.get_optional<PrimitiveInt>(2, "i2");
  ASSERT_NE(i2a, std::nullopt);
  EXPECT_EQ(i2a, 2);

  const auto i2b = fcp_.get_optional<PrimitiveInt>(3, "i2");
  ASSERT_NE(i2b, std::nullopt);
  EXPECT_EQ(i2b, 2);
}

TEST_F(FieldCallParamsTest, TestGetWithInvalidType) {
  SCOPED_TRACE(testing::Message() << "fcp_= " << fcp_);

  EXPECT_THROW((void)fcp_.get<PrimitiveBool>(0, "s1"), ArgumentTypeError);
  EXPECT_THROW((void)fcp_.get<PrimitiveBool>(1, "i1"), ArgumentTypeError);
  EXPECT_THROW((void)fcp_.get<PrimitiveBool>(2, "s2"), ArgumentTypeError);
  EXPECT_THROW((void)fcp_.get<PrimitiveBool>(3, "i2"), ArgumentTypeError);
}

TEST_F(FieldCallParamsTest, TestGetOptionalWithInvalidType) {
  SCOPED_TRACE(testing::Message() << "fcp_= " << fcp_);

  EXPECT_THROW((void)fcp_.get_optional<PrimitiveBool>(0, "s1"),
               ArgumentTypeError);
  EXPECT_THROW((void)fcp_.get_optional<PrimitiveBool>(1, "i1"),
               ArgumentTypeError);
  EXPECT_THROW((void)fcp_.get_optional<PrimitiveBool>(2, "s2"),
               ArgumentTypeError);
  EXPECT_THROW((void)fcp_.get_optional<PrimitiveBool>(3, "i2"),
               ArgumentTypeError);
}

TEST_F(FieldCallParamsTest, TestGetWithMissingArgument) {
  SCOPED_TRACE(testing::Message() << "fcp_= " << fcp_);

  EXPECT_THROW((void)fcp_.get<PrimitiveBool>(4, "s3"), ArgumentMissingError);
}

TEST_F(FieldCallParamsTest, TestGetOptionalWithMissingArgument) {
  SCOPED_TRACE(testing::Message() << "fcp_= " << fcp_);

  EXPECT_EQ(fcp_.get_optional<PrimitiveBool>(4, "s3"), std::nullopt);
}

TEST_F(FieldCallParamsTest, TestEq) {
  SCOPED_TRACE(testing::Message() << "fcp_= " << fcp_);

  auto fcp_copy = fcp_;
  SCOPED_TRACE(testing::Message() << "fcp_copy = " << fcp_copy);
  EXPECT_TRUE(fcp_ == fcp_);
  EXPECT_TRUE(fcp_copy == fcp_);
  EXPECT_TRUE(fcp_ == fcp_copy);
  EXPECT_FALSE(fcp_ != fcp_);
  EXPECT_FALSE(fcp_copy != fcp_);
  EXPECT_FALSE(fcp_ != fcp_copy);

  auto fcp_different_pos = fcp_;
  fcp_different_pos.pos_params()[1] = PrimitiveInt{2};
  SCOPED_TRACE(testing::Message()
               << "fcp_different_pos = " << fcp_different_pos);

  EXPECT_FALSE(fcp_different_pos == fcp_);
  EXPECT_FALSE(fcp_ == fcp_different_pos);
  EXPECT_TRUE(fcp_different_pos != fcp_);
  EXPECT_TRUE(fcp_ != fcp_different_pos);

  auto fcp_different_named = fcp_;
  fcp_different_named.named_params().emplace("i3", PrimitiveInt{0});
  SCOPED_TRACE(testing::Message()
               << "fcp_different_named = " << fcp_different_named);

  EXPECT_FALSE(fcp_different_named == fcp_);
  EXPECT_FALSE(fcp_ == fcp_different_named);
  EXPECT_TRUE(fcp_different_named != fcp_);
  EXPECT_TRUE(fcp_ != fcp_different_named);

  EXPECT_FALSE(fcp_different_named == fcp_different_pos);
  EXPECT_FALSE(fcp_different_pos == fcp_different_named);
  EXPECT_TRUE(fcp_different_named != fcp_different_pos);
  EXPECT_TRUE(fcp_different_pos != fcp_different_named);
}

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
