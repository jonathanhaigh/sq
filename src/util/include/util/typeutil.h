#ifndef SQ_INCLUDE_GUARD_util_typeutil_h_
#define SQ_INCLUDE_GUARD_util_typeutil_h_

#include <type_traits>

namespace sq::util {

template <typename T>
using disable_lvalues_t = std::enable_if_t<!std::is_lvalue_reference_v<T>>;

} // namespace sq::util

#endif // SQ_INCLUDE_GUARD_util_typeutil_h_
