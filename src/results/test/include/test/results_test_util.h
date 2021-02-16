#ifndef SQ_INCLUDE_GUARD_results_test_results_test_util_h_
#define SQ_INCLUDE_GUARD_results_test_results_test_util_h_

#include "results/results.h"
#include "test/Primitive_test_util.h"

#include <cassert>
#include <functional>
#include <gmock/gmock.h>
#include <optional>

namespace sq::test {

using namespace sq::results;

using testing::Return;
using testing::ByMove;

using Data = ResultTree::Data;
using ObjData = ResultTree::ObjData;
using ObjDataField = ObjData::value_type;
using ArrayData = ResultTree::ArrayData;

template <typename T>
static constexpr bool is_result_tree_data_v = std::disjunction_v<
    std::is_same<T, Data>,
    std::is_same<T, ObjData>,
    std::is_same<T, ArrayData>,
    std::is_same<T, Primitive>
>;

template <typename T>
using enable_if_result_tree_data_t = std::enable_if_t<is_result_tree_data_v<T>>;

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
    using ResultGenerator = std::function<
        Result(std::string_view, const FieldCallParams&)
    >;

    template <typename T, typename = enable_if_primitive_like_t<T>>
    FakeField(ResultGenerator result_generator, T&& primitive);

    template <typename T, typename = enable_if_primitive_like_t<T>>
    FakeField(T&& primitive);

    FakeField(Result&& result);
    FakeField(ResultGenerator result_generator);
    FakeField();
    FakeField(const FakeField&) = delete;
    FakeField(FakeField&&) = delete;
    FakeField& operator=(const FakeField&) = delete;
    FakeField& operator=(FakeField&&) = delete;
    ~FakeField() noexcept = default;

    Result get(
        const std::string_view member,
        const FieldCallParams& params
    ) const override;

    Primitive to_primitive() const override;

private:
    ResultGenerator result_generator_;
    Primitive primitive_ = PrimitiveInt{0};
};

/**
 * Place an expectation on a mock field that its to_primitive() method will be
 * called exactly once and will return the given value.
 */
template <typename T, typename = enable_if_primitive_like_t<T>>
void expect_one_primitive_access(MockField& mf, T&& retval);

/**
 * Place an expectation on a mock field that its fields will be called
 * according to the given call specifications.
 */
void expect_field_accesses(
    MockField& mf,
    const std::string_view field_name,
    const FieldCallParams& params,
    Result&& retval
);

/**
 * Place an expectation on a mock field that its fields will be called
 * according to the given call specifications.
 */
template <typename... Args>
void expect_field_accesses(
    MockField& mf,
    const std::string_view field_name,
    const FieldCallParams& params,
    Result&& retval,
    Args&&... args
);

/**
 * Create a MockField whose only access is expected to be one call to
 * to_primitive().
 */
template <typename T, typename = enable_if_primitive_like_t<T>>
StrictMockFieldPtr field_with_one_primitive_access(T&& retval);

/**
 * Create a MockField whose only accesses are expected to be the calls in the
 * given specs.
 */
template <typename... Args>
static StrictMockFieldPtr field_with_accesses(Args&&... args);

/**
 * Create a ResultTree representing an object with the given field names and
 * values.
 */
template <typename... Args>
ResultTree obj_data_tree(Args&&... args);

/**
 * Create a ResultTree representing an array with the given values.
 */
template <typename... Args>
ResultTree array_data_tree(Args&&... args);

template <typename T, typename = enable_if_primitive_like_t<T>>
ResultTree primitive_tree(T&& primitive);

static constexpr auto input = ranges::category::input;
static constexpr auto forward = ranges::category::forward;
static constexpr auto bidirectional = ranges::category::bidirectional;
static constexpr auto random_access = ranges::category::random_access;
static constexpr auto sized = ranges::category::sized;

extern const std::initializer_list<ranges::category> all_categories;

template <typename T>
Result to_field_range(const ranges::category cat, T&& rng);

} // namespace sq::test

namespace ranges  {

std::ostream& operator<<(std::ostream& os, const category& cat);

} // namespace ranges

namespace sq::results {

std::ostream& operator<<(std::ostream& os, const ResultTree& tree);

} // namespace sq::results

#include "results_test_util.inl.h"

#endif // SQ_INCLUDE_GUARD_results_test_results_test_util_h_
