#ifndef SQ_INCLUDE_GUARD_util_SharedRange_h_
#define SQ_INCLUDE_GUARD_util_SharedRange_h_

#include <gsl/gsl>
#include <memory>
#include <range/v3/range/concepts.hpp>
#include <vector>

namespace sq::util {

template <typename R>
requires ranges::cpp20::range<R>
struct SharedRange {
public:
  explicit SharedRange(const std::shared_ptr<R> &base) : base_{base} {}

  explicit SharedRange(std::shared_ptr<R> &&base) : base_{std::move(base)} {}

  constexpr SharedRange() noexcept = default;
  SharedRange(const SharedRange &) noexcept = default;
  SharedRange(SharedRange &&) noexcept = default;
  SharedRange &operator=(const SharedRange &) noexcept = default;
  SharedRange &operator=(SharedRange &&) noexcept = default;
  ~SharedRange() noexcept = default;

  auto begin() {
    Expects(base_ != nullptr);
    return ranges::begin(*base_);
  }
  auto end() {
    Expects(base_ != nullptr);
    return ranges::end(*base_);
  }

private:
  // views must be semiregular, which means that SharedRange must have a
  // default constructor, which, unfortunately, means that we can't use
  // gsl::not_null here.
  std::shared_ptr<R> base_ = nullptr;
};

} // namespace sq::util

namespace ranges {

template <typename R>
inline constexpr bool enable_view<sq::util::SharedRange<R>> = true;

} // namespace ranges

namespace sq::util {

static_assert(ranges::cpp20::range<SharedRange<std::vector<int>>>);
static_assert(ranges::cpp20::view<SharedRange<std::vector<int>>>);
static_assert(ranges::viewable_range<SharedRange<std::vector<int>>>);

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_SharedRange_h_
