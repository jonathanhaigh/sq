#include "system/CacheingField.h"

#include "util/ASSERT.h"
#include "util/typeutil.h"

#include <range/v3/range/concepts.hpp>

namespace sq::system {
namespace {

/**
 * Get whether the results for a field should be cached or not.
 */
struct ShouldCache
{
    SQ_ND bool operator()(const Result& value) const
    {
        return std::visit(*this, value);
    }

    SQ_ND bool operator()(SQ_MU const FieldPtr& value) const
    {
        return true;
    }

    SQ_ND bool operator()(SQ_MU const ranges::cpp20::range auto& rng) const
    {
        // Don't cache input_ranges - they can only be iterated over once.
        return ranges::forward_range<decltype(rng)>;
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
