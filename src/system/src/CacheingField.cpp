#include "system/CacheingField.h"

#include "util/ASSERT.h"

#include <range/v3/range/concepts.hpp>

namespace sq::system {
namespace {

/**
 * Get whether the results for a field should be cached or not.
 */
struct ShouldCache
{
    [[nodiscard]] bool operator()(const Result& value) const
    {
        return std::visit(*this, value);
    }

    [[nodiscard]] bool operator()([[maybe_unused]] const FieldPtr& value) const
    {
        return true;
    }

    template <ranges::cpp20::range R>
    [[nodiscard]] bool operator()([[maybe_unused]] const R&) const
    {
        // Don't cache input_ranges - they can only be iterated over once.
        return ranges::forward_range<R>;
    }
};

} // namespace

Result CacheingField::get(
    std::string_view member,
    const FieldCallParams& params
) const
{
    if (auto it = cache_.find(member); it != cache_.end())
    {
        return it->second;;
    }
    auto result = dispatch(member, params);
    if (!ShouldCache{}(result))
    {
        return result;
    }
    const auto [it, inserted] = cache_.emplace(member, std::move(result));
    ASSERT(inserted);
    ASSERT(it != cache_.end());
    return it->second;
}

} // namespace sq::system
