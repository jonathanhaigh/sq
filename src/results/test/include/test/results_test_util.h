#ifndef SQ_INCLUDE_GUARD_results_test_results_test_util_h_
#define SQ_INCLUDE_GUARD_results_test_results_test_util_h_

#include "results/results.h"
#include "test/Primitive_test_util.h"

#include <functional>
#include <gmock/gmock.h>
#include <optional>

namespace sq::test {

using ::testing::Return;
using ::testing::ByMove;
using ::testing::_;

using sq::results::ResultTree;
using Data = ResultTree::Data;
using ObjData = ResultTree::ObjData;
using ObjDataField = ObjData::value_type;
using ArrayData = ResultTree::ArrayData;

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

    template <util::ConvertibleToAlternative<Primitive> T>
    FakeField(ResultGenerator result_generator, T&& primitive);

    template <util::ConvertibleToAlternative<Primitive> T>
    FakeField(T&& primitive);

    FakeField(Result&& result);
    FakeField(ResultGenerator result_generator);
    FakeField();
    FakeField(const FakeField&) = delete;
    FakeField(FakeField&&) = delete;
    FakeField& operator=(const FakeField&) = delete;
    FakeField& operator=(FakeField&&) = delete;
    ~FakeField() noexcept = default;

    [[nodiscard]] Result get(
        std::string_view member,
        const FieldCallParams& params
    ) const override;

    [[nodiscard]] Primitive to_primitive() const override;

private:
    ResultGenerator result_generator_;
    Primitive primitive_ = PrimitiveInt{0};
};

/**
 * Place an expectation on a mock field that its to_primitive() method will be
 * called exactly once and will return the given value.
 */
template <util::ConvertibleToAlternative<Primitive> T>
void expect_one_primitive_access(MockField& mf, T&& retval);

/**
 * Place an expectation on a mock field that its fields will be called
 * according to the given call specifications.
 */
void expect_field_accesses(
    MockField& mf,
    std::string_view field_name,
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
    std::string_view field_name,
    const FieldCallParams& params,
    Result&& retval,
    Args&&... args
);

/**
 * Create a MockField whose only access is expected to be one call to
 * to_primitive().
 */
template <util::ConvertibleToAlternative<Primitive> T>
[[nodiscard]] StrictMockFieldPtr field_with_one_primitive_access(T&& retval);

/**
 * Create a MockField whose only accesses are expected to be the calls in the
 * given specs.
 */
template <typename... Args>
[[nodiscard]] StrictMockFieldPtr field_with_accesses(Args&&... args);

/**
 * Create a ResultTree representing an object with the given field names and
 * values.
 */
template <typename... Args>
[[nodiscard]] ResultTree obj_data_tree(Args&&... args);

/**
 * Create a ResultTree representing an array with the given values.
 */
template <util::ConvertibleToAlternative<ResultTree::Data>... Args>
[[nodiscard]] ResultTree array_data_tree(Args&&... args);

template <util::ConvertibleToAlternative<Primitive> T>
[[nodiscard]] ResultTree primitive_tree(T&& primitive);

inline constexpr auto input = ranges::category::input;
inline constexpr auto forward = ranges::category::forward;
inline constexpr auto bidirectional = ranges::category::bidirectional;
inline constexpr auto random_access = ranges::category::random_access;
inline constexpr auto sized = ranges::category::sized;

inline constexpr auto all_categories = {
    input, input|sized,
    forward, forward|sized,
    bidirectional, bidirectional|sized,
    random_access, random_access|sized
};


template <ranges::cpp20::view T>
[[nodiscard]] Result to_field_range(ranges::category cat, T&& rng);

} // namespace sq::test

namespace ranges  {

std::ostream& operator<<(std::ostream& os, const category& cat);

} // namespace ranges

namespace sq::results {

std::ostream& operator<<(std::ostream& os, const ResultTree& tree);

} // namespace sq::results

#include "results_test_util.inl.h"

#endif // SQ_INCLUDE_GUARD_results_test_results_test_util_h_
