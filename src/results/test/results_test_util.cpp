#include "test/results_test_util.h"

namespace sq::test {

FakeField::FakeField(Result&& result)
    : result_generator_{
        [&](auto, auto) { return std::move(result); }
    }
{ }

FakeField::FakeField(ResultGenerator result_generator)
    : result_generator_{result_generator}
{ }

FakeField::FakeField()
    : FakeField{PrimitiveInt{0}}
{ }

Result FakeField::get(
    const std::string_view member,
    const FieldCallParams& params
) const
{
    return result_generator_(member, params);
}

Primitive FakeField::to_primitive() const
{
    return primitive_;
}

void expect_field_accesses(
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

namespace detail {

void add_fields_to_obj_data(
    ObjData& obj,
    const std::string_view field_name,
    ResultTree&& field_data
)
{
    obj.emplace_back(field_name, std::move(field_data));
}

void add_items_to_array_data(ArrayData& arr, ResultTree&& field_data)
{
    arr.emplace_back(std::move(field_data));
}

} // namespace detail

constexpr std::initializer_list<ranges::category> all_categories = {
    input, input|sized,
    forward, forward|sized,
    bidirectional, bidirectional|sized,
    random_access, random_access|sized
};

} // namespace sq::test

namespace ranges {

std::ostream& operator<<(std::ostream& os, const category& cat)
{
    if ((cat & category::sized) == category::sized)
    {
        os << "sized ";
    }
    if ((cat & category::mask) == category::random_access) 
    {
        os << "random_access";
    }
    else if((cat & category::mask) == category::bidirectional)
    {
        os << "bidirectional";
    }
    else if ((cat & category::mask) == category::forward)
    {
        os << "forward";
    }
    else if ((cat & category::mask) == category::input)
    {
        os << "input";
    }
    else
    {
        os << "unknown";
    }
    return os;
}

} // namespace ranges