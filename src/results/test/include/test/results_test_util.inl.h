#ifndef SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_
#define SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_

#include "test/Primitive_test_util.h"

namespace sq::test {

template <typename T, typename>
FakeField::FakeField(ResultGenerator result_generator, T&& primitive)
    : result_generator_{result_generator}
    , primitive_{test::to_primitive(std::forward<T>(primitive))}
{ }

template <typename T, typename>
FakeField::FakeField(T&& primitive)
    : result_generator_{
        [&](auto, auto) { return std::make_unique<FakeField>(); }
    }
    , primitive_{test::to_primitive(std::forward<T>(primitive))}
{ }

template <typename T, typename>
void expect_one_primitive_access(MockField& mf, T&& retval)
{
    EXPECT_CALL(mf, to_primitive())
        .Times(1)
        .WillOnce(Return(to_primitive(std::forward<T>(retval))));
}

template <typename... Args>
void expect_field_accesses(
    MockField& mf,
    const std::string_view field_name,
    const FieldCallParams& params,
    Result&& retval,
    Args&&... args
)
{
    expect_field_accesses(mf, field_name, params, std::move(retval));
    expect_field_accesses(mf, std::forward<Args>(args)...);
}

template <typename T, typename>
StrictMockFieldPtr field_with_one_primitive_access(T&& retval)
{
    auto mf = std::make_unique<testing::StrictMock<MockField>>();
    expect_one_primitive_access(*mf, std::forward<T>(retval));
    return mf;
}

template <typename... Args>
static StrictMockFieldPtr field_with_accesses(Args&&... args)
{
    auto mf = std::make_unique<StrictMockField>();
    expect_field_accesses(*mf, std::forward<Args>(args)...);
    return mf;
}

namespace detail {

void add_fields_to_obj_data(
    ObjData& obj,
    std::string_view field_name,
    ResultTree&& field_data
);

template <typename T, typename = enable_if_primitive_like_t<T>>
void add_fields_to_obj_data(
    ObjData& obj,
    std::string_view field_name,
    T&& field_data
)
{
    add_fields_to_obj_data(
        obj,
        field_name,
        ResultTree{to_primitive(std::forward<T>(field_data))}
    );
}

template <
    typename T,
    typename = enable_if_result_tree_data_t<T>,
    typename = void // to distinguish from primitive_like version
>
void add_fields_to_obj_data(
    ObjData& obj,
    std::string_view field_name,
    T&& field_data
)
{
    add_fields_to_obj_data(
        obj,
        field_name,
        ResultTree{std::forward<T>(field_data)}
    );
}

template <typename T, typename... Args>
void add_fields_to_obj_data(
    ObjData& obj,
    const std::string_view field_name,
    T&& field_data,
    Args&&... args
)
{
    add_fields_to_obj_data(obj, field_name, std::forward<T>(field_data));
    add_fields_to_obj_data(obj, std::forward<Args>(args)...);
}

void add_items_to_array_data(ArrayData& arr, ResultTree&& field_data);

template <typename T, typename = enable_if_primitive_like_t<T>>
void add_items_to_array_data(ArrayData& arr, T&& field_data)
{
    add_items_to_array_data(
        arr, 
        ResultTree{to_primitive(std::forward<T>(field_data))}
    );
}

template <
    typename T,
    typename = enable_if_result_tree_data_t<T>,
    typename = void // To distinguish from primitive_like version
>
void add_items_to_array_data(ArrayData& arr, T&& field_data)
{
    add_items_to_array_data(arr, ResultTree{std::forward<T>(field_data)});
}

template <typename T, typename... Args>
void add_items_to_array_data(
    ArrayData& arr,
    T&& field_data,
    Args&&... args
)
{
    add_items_to_array_data(arr, std::forward<T>(field_data));
    add_items_to_array_data(arr, std::forward<Args>(args)...);
}

} // namespace detail

template <typename... Args>
ResultTree obj_data_tree(Args&&... args)
{
    auto obj_data = ObjData{};
    detail::add_fields_to_obj_data(obj_data, std::forward<Args>(args)...);
    return ResultTree{std::move(obj_data)};
}

template <typename... Args>
ResultTree array_data_tree(Args&&... args)
{
    auto arr = ArrayData{};
    detail::add_items_to_array_data(arr, std::forward<Args>(args)...);
    return ResultTree{std::move(arr)};
}

template <typename T, typename>
ResultTree primitive_tree(T&& primitive)
{
    return ResultTree{to_primitive(std::forward<T>(primitive))};
}

template <typename T>
Result to_field_range(const ranges::category cat, T&& rng)
{
    if (cat == input)
    {
        return FieldRange<input>{std::forward<T>(rng)};
    }
    if (cat == (input|sized))
    {
        return FieldRange<input|sized>{std::forward<T>(rng)};
    }
    if (cat == forward)
    {
        return FieldRange<forward>{std::forward<T>(rng)};
    }
    if (cat == (forward|sized))
    {
        return FieldRange<forward|sized>{std::forward<T>(rng)};
    }
    if (cat == bidirectional)
    {
        return FieldRange<bidirectional>{std::forward<T>(rng)};
    }
    if (cat == (bidirectional|sized))
    {
        return FieldRange<bidirectional|sized>{std::forward<T>(rng)};
    }
    if (cat == random_access)
    {
        return FieldRange<random_access>{std::forward<T>(rng)};
    }
    if (cat == (random_access|sized))
    {
        return FieldRange<random_access|sized>{std::forward<T>(rng)};
    }
    assert(false);
    return FieldRange<input>{std::forward<T>(rng)};
}

} // namespace test

#endif // SQ_INCLUDE_GUARD_results_test_results_test_util_inl_h_
