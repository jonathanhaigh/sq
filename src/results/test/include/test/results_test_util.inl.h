/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_
#define SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_

#include "test/Primitive_test_util.h"
#include "util/ASSERT.h"
#include "util/typeutil.h"

namespace sq::test {

FakeField::FakeField(ResultGenerator result_generator, PrimitiveLike auto&& primitive)
    : result_generator_{result_generator}
    , primitive_{test::to_primitive(SQ_FWD(primitive))}
{ }

FakeField::FakeField(PrimitiveLike auto&& primitive)
    : result_generator_{
        [&](auto, auto) { return std::make_shared<FakeField>(); }
    }
    , primitive_{test::to_primitive(SQ_FWD(primitive))}
{ }

void expect_one_primitive_access(MockField& mf, PrimitiveLike auto&& retval)
{
    EXPECT_CALL(mf, to_primitive())
        .Times(1)
        .WillOnce(Return(to_primitive(SQ_FWD(retval))));
}

void expect_field_accesses(
    MockField& mf,
    std::string_view field_name,
    const FieldCallParams& params,
    Result&& retval,
    auto&&... args
)
{
    expect_field_accesses(mf, field_name, params, std::move(retval));
    expect_field_accesses(mf, SQ_FWD(args)...);
}

StrictMockFieldPtr field_with_one_primitive_access(PrimitiveLike auto&& retval)
{
    auto mf = std::make_shared<testing::StrictMock<MockField>>();
    expect_one_primitive_access(*mf, SQ_FWD(retval));
    return mf;
}

StrictMockFieldPtr field_with_accesses(auto&&... args)
{
    auto mf = std::make_shared<StrictMockField>();
    expect_field_accesses(*mf, SQ_FWD(args)...);
    return mf;
}

namespace detail {

void add_fields_to_obj_data(
    ObjData& obj,
    std::string_view field_name,
    ResultTree&& field_data
);

void add_fields_to_obj_data(
    ObjData& obj,
    std::string_view field_name,
    PrimitiveLike auto&& field_data
)
{
    add_fields_to_obj_data(
        obj,
        field_name,
        ResultTree{to_primitive(SQ_FWD(field_data))}
    );
}

void add_fields_to_obj_data(
    ObjData& obj,
    std::string_view field_name,
    ResultTreeDataAlternative auto&& field_data
)
{
    add_fields_to_obj_data(
        obj,
        field_name,
        ResultTree{SQ_FWD(field_data)}
    );
}

void add_fields_to_obj_data(
    ObjData& obj,
    std::string_view field_name,
    auto&& field_data,
    auto&&... args
)
{
    add_fields_to_obj_data(obj, field_name, SQ_FWD(field_data));
    add_fields_to_obj_data(obj, SQ_FWD(args)...);
}

void add_items_to_array_data(ArrayData& arr, ResultTree&& field_data);

void add_items_to_array_data(ArrayData& arr, PrimitiveLike auto&& field_data)
{
    add_items_to_array_data(
        arr, 
        ResultTree{to_primitive(SQ_FWD(field_data))}
    );
}

void add_items_to_array_data(ArrayData& arr, ResultTreeDataAlternative auto&& field_data)
{
    add_items_to_array_data(arr, ResultTree{SQ_FWD(field_data)});
}

template <util::ConvertibleToAlternative<ResultTree::Data> T, typename... Args>
void add_items_to_array_data(
    ArrayData& arr,
    T&& field_data,
    Args&&... args
)
{
    add_items_to_array_data(arr, SQ_FWD(field_data));
    add_items_to_array_data(arr, SQ_FWD(args)...);
}

} // namespace detail

ResultTree obj_data_tree(auto&&... args)
{
    auto obj_data = ObjData{};
    detail::add_fields_to_obj_data(obj_data, SQ_FWD(args)...);
    return ResultTree{std::move(obj_data)};
}

template <util::ConvertibleToAlternative<ResultTree::Data>... Args>
ResultTree array_data_tree(Args&&... args)
{
    auto arr = ArrayData{};
    detail::add_items_to_array_data(arr, SQ_FWD(args)...);
    return ResultTree{std::move(arr)};
}

ResultTree primitive_tree(PrimitiveLike auto&& primitive)
{
    return ResultTree{to_primitive(SQ_FWD(primitive))};
}

Result to_field_range(ranges::category cat, ranges::cpp20::view auto&& rng)
{
    if (cat == input)
    {
        return FieldRange<input>{SQ_FWD(rng)};
    }
    if (cat == (input|sized))
    {
        return FieldRange<input|sized>{SQ_FWD(rng)};
    }
    if (cat == forward)
    {
        return FieldRange<forward>{SQ_FWD(rng)};
    }
    if (cat == (forward|sized))
    {
        return FieldRange<forward|sized>{SQ_FWD(rng)};
    }
    if (cat == bidirectional)
    {
        return FieldRange<bidirectional>{SQ_FWD(rng)};
    }
    if (cat == (bidirectional|sized))
    {
        return FieldRange<bidirectional|sized>{SQ_FWD(rng)};
    }
    if (cat == random_access)
    {
        return FieldRange<random_access>{SQ_FWD(rng)};
    }
    if (cat == (random_access|sized))
    {
        return FieldRange<random_access|sized>{SQ_FWD(rng)};
    }
    ASSERT(false);
    return FieldRange<input>{SQ_FWD(rng)};
}

} // namespace test

#endif // SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_
