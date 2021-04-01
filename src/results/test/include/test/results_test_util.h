#ifndef SQ_INCLUDE_GUARD_results_test_results_test_util_h_
#define SQ_INCLUDE_GUARD_results_test_results_test_util_h_

#include "core/typeutil.h"
#include "results/results.h"
#include "test/Primitive_test_util.h"

#include <functional>
#include <gmock/gmock.h>
#include <optional>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>

namespace sq::test {

namespace rv = ranges::views;

using ::testing::_;
using ::testing::ByMove;
using ::testing::Return;

/**
 * Represents a Field upon whose accesses we have expectations.
 */
struct MockField : Field {
  MOCK_METHOD(Result, get,
              (std::string_view member, const FieldCallParams &params),
              (const, override));

  // Don't mock to_primitive() - we don't really care when it gets called - it
  // should be cheap.
  Primitive to_primitive() const override { return 0; }
};

/**
 * A mock field that complains if an unexpected method call happens.
 */
using StrictMockField = testing::StrictMock<MockField>;
using StrictMockFieldPtr = std::shared_ptr<StrictMockField>;

/**
 * A fixture for tests that use MockFields.
 */
struct MockFieldTest : public ::testing::Test {
  MockFieldTest() {
    ::testing::DefaultValue<Result>::SetFactory(
        []() { return Result{std::make_shared<MockField>()}; });
    ::testing::DefaultValue<FieldPtr>::SetFactory(
        []() { return FieldPtr{std::make_shared<MockField>()}; });
  }
};

/**
 * Represents a field upon whose accesses we *dont* have expectations.
 *
 * Really, this represents a whole system upon whose accesses we don't have
 * expectations because its get() function just creates another FakeField.
 */
struct FakeField : Field {
  using ResultGenerator =
      std::function<Result(std::string_view, const FieldCallParams &)>;

  FakeField(ResultGenerator result_generator, PrimitiveLike auto &&primitive);
  FakeField(PrimitiveLike auto &&primitive);
  FakeField(Result &&result);
  FakeField(ResultGenerator result_generator);
  FakeField();
  FakeField(const FakeField &) = delete;
  FakeField(FakeField &&) = delete;
  FakeField &operator=(const FakeField &) = delete;
  FakeField &operator=(FakeField &&) = delete;
  ~FakeField() noexcept = default;

  SQ_ND Result get(std::string_view member,
                   const FieldCallParams &params) const override;

  SQ_ND Primitive to_primitive() const override;

private:
  ResultGenerator result_generator_;
  Primitive primitive_ = PrimitiveInt{0};
};

SQ_ND FieldPtr fake_field(auto &&...args);

SQ_ND Result
fake_field_range(PrimitiveInt start, PrimitiveInt stop,
                 ranges::category cat = ranges::category::random_access);

/**
 * Place an expectation on a mock field that its fields will be called
 * according to the given call specifications.
 */
void expect_field_accesses(MockField &mf, std::string_view field_name,
                           const FieldCallParams &params, Result &&retval);

/**
 * Place an expectation on a mock field that its fields will be called
 * according to the given call specifications.
 */
void expect_field_accesses(MockField &mf, std::string_view field_name,
                           const FieldCallParams &params, Result &&retval,
                           auto &&...args);

/**
 * Create a MockField whose only accesses are expected to be the calls in the
 * given specs.
 */
SQ_ND StrictMockFieldPtr field_with_accesses(auto &&...args);

/**
 * Create a MockField with no expected accesses.
 */
SQ_ND StrictMockFieldPtr field_with_no_accesses();

inline constexpr auto input = ranges::category::input;
inline constexpr auto forward = ranges::category::forward;
inline constexpr auto bidirectional = ranges::category::bidirectional;
inline constexpr auto random_access = ranges::category::random_access;
inline constexpr auto sized = ranges::category::sized;

inline constexpr auto all_categories = {
    input,         input | sized,         forward,       forward | sized,
    bidirectional, bidirectional | sized, random_access, random_access | sized};

SQ_ND Result to_field_range(ranges::category cat,
                            ranges::cpp20::view auto &&rng);

/**
 * Assert that two JSON strings encode the same value.
 */
void expect_equivalent_json(std::string_view json1, std::string_view json2);

} // namespace sq::test

namespace ranges {

std::ostream &operator<<(std::ostream &os, const category &cat);

} // namespace ranges

#include "results_test_util.inl.h"

#endif // SQ_INCLUDE_GUARD_results_test_results_test_util_h_
