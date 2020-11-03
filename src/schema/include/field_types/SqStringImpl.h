#ifndef SQ_INCLUDE_GUARD_field_types_SqStringImpl_h_
#define SQ_INCLUDE_GUARD_field_types_SqStringImpl_h_

#include <string>

namespace sq {
namespace field_types {

class SqStringImpl
{
public:
    explicit SqStringImpl(const std::string& str);

private:
    std::string string_;
    friend class SqString;
};

} // namespace field_types
} // namespace sq

#endif // SQ_INCLUDE_GUARD_field_types_SqStringImpl_h_

