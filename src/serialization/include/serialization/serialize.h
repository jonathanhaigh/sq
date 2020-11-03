#ifndef SQ_INCLUDE_GUARD_serialization_serialize_h_
#define SQ_INCLUDE_GUARD_serialization_serialize_h_

#include "results/results.h"

#include <iosfwd>

namespace sq::serialization {

void serialize_results(std::ostream& os, const results::ResultTree& results);

} // namespace sq::serialization

#endif // SQ_INCLUDE_GUARD_serialization_serialize_h_
