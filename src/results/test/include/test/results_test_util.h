#ifndef SQ_INCLUDE_GUARD_results_test_results_test_util_h_
#define SQ_INCLUDE_GUARD_results_test_results_test_util_h_

#include "results/results.h"

#include <gmock/gmock.h>

namespace sq::test {

using namespace sq::results;

using testing::Return;
using testing::ByMove;

using Data = ResultTree::Data;
using ObjData = ResultTree::ObjData;
using ObjDataField = ObjData::value_type;

/**
 * Represents a Field upon whose accesses we have expectations.
 */
struct MockField
    : Field
{
    MOCK_METHOD(
        Result,
        get,
        (std::string_view member, const FieldCallParams& params),
        (const, override)
    );
    MOCK_METHOD(Primitive, to_primitive, (), (const));
};

/**
 * A mock field that complains if an unexpected method call happens.
 */
using StrictMockField = testing::StrictMock<MockField>;
using StrictMockFieldPtr = std::unique_ptr<StrictMockField>;

/**
 * Represents a field upon whose accesses we *dont* have expectations.
 *
 * Really, this represents a whole system upon whose accesses we don't have
 * expectations because its get() function just creates another FakeField.
 */
struct FakeField
    : Field
{
    Result get(
        [[maybe_unused]] std::string_view member,
        [[maybe_unused]] const FieldCallParams& params
    ) const override
    {
        return std::make_unique<FakeField>();
    }

    Primitive to_primitive() const override
    {
        return PrimitiveInt{0};
    }
};

/**
 * Place an expectation on a mock field that its to_primitive() method will be
 * called exactly once and will return the given value.
 */
static void expect_one_primitive_access(MockField& mf, const Primitive& retval)
{
    EXPECT_CALL(mf, to_primitive())
        .Times(1)
        .WillOnce(Return(retval));
}

/**
 * Place an expectation on a mock field that its fields will be called
 * according to the given call specifications.
 */
static void expect_field_accesses(
    MockField& mf,
    const std::string_view field_name,
    const FieldCallParams& params,
    Result&& retval
)
{
    EXPECT_CALL(mf, get(field_name, params))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(retval))));
}

/**
 * Place an expectation on a mock field that its fields will be called
 * according to the given call specifications.
 */
template <typename... Args>
static void expect_field_accesses(
    MockField& mf,
    const std::string_view field_name,
    const FieldCallParams& params,
    Result&& retval,
    Args&&... args
)
{
    expect_field_accesses(mf, field_name, params, std::move(retval));
    expect_field_accesses(mf, std::forward<Args>(args)...);
}

/**
 * Create a MockField whose only access is expected to be one call to
 * to_primitive().
 */
static StrictMockFieldPtr field_with_one_primitive_access(
    const Primitive& retval
)
{
    auto mf = std::make_unique<testing::StrictMock<MockField>>();
    expect_one_primitive_access(*mf, retval);
    return mf;
}

/**
 * Create a MockField whose only accesses are expected to be the calls in the
 * given specs.
 */
template <typename... Args>
static StrictMockFieldPtr field_with_accesses(Args&&... args)
{
    auto mf = std::make_unique<StrictMockField>();
    expect_field_accesses(*mf, std::forward<Args>(args)...);
    return mf;
}

static void add_fields_to_obj_data(
    ObjData& obj,
    const std::string_view field_name,
    ResultTree&& field_data
)
{
    obj.emplace_back(field_name, std::move(field_data));
}

template <typename... Args>
static void add_fields_to_obj_data(
    ObjData& obj,
    const std::string_view field_name,
    ResultTree&& field_data,
    Args&&... args
)
{
    add_fields_to_obj_data(obj, field_name, std::move(field_data));
    add_fields_to_obj_data(obj, std::forward<Args>(args)...);
}

/**
 * Create a ResultTree representing an object with the given field names and
 * values.
 */
template <typename... Args>
static ResultTree obj_data_tree(Args&&... args)
{
    auto obj_data = ObjData{};
    add_fields_to_obj_data(obj_data, std::forward<Args>(args)...);
    return ResultTree{std::move(obj_data)};
}

} // namespace sq::test

#endif // SQ_INCLUDE_GUARD_results_test_results_test_util_h_
