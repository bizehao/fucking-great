//
// Created by Qiyu on 17-6-5.
//

#ifndef IGUANA_REFLECTION_HPP
#define IGUANA_REFLECTION_HPP
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <iomanip>
#include <map>
#include <vector>
#include <array>
#include <type_traits>
#include <functional>
#include <string_view>

#include "detail/itoa.hpp"
#include "detail/traits.hpp"
#include "detail/string_stream.hpp"
#include "meta_macro.hpp"

namespace iguana::detail {

#define ADD_VIEW(str) std::string_view(#str, sizeof(#str) - 1)

#define ADD_REFERENCE(t) std::reference_wrapper<decltype(t)>(t)
#define ADD_REFERENCE_CONST(t) std::reference_wrapper<std::add_const_t<decltype(t)>>(t)
#define FIELD(t) t

#define MAKE_ARG_LIST(N, op, arg, ...) _MACRO_CONCAT(_ARG_OP_, N)(op, _ARG_COMMA, arg, ##__VA_ARGS__)

#define GET_ARG_COUNT(...) _ARG_COUNT(__VA_ARGS__)

#define MAKE_STR_LIST(...) FOR_EACH_COMMA(ADD_VIEW, __VA_ARGS__)

#define MAKE_META_DATA_IMPL(STRUCT_NAME, ...)                                                                                                        \
    static auto iguana_reflect_members(STRUCT_NAME const&) {                                                                                         \
        struct reflect_members {                                                                                                                     \
            constexpr static decltype(auto) apply_impl() { return std::make_tuple(__VA_ARGS__); }                                                    \
            using type = void;                                                                                                                       \
            using size_type = std::integral_constant<size_t, GET_ARG_COUNT(__VA_ARGS__)>;                                                            \
            constexpr static std::string_view name() { return std::string_view(#STRUCT_NAME, sizeof(#STRUCT_NAME) - 1); }                            \
            constexpr static size_t value() { return size_type::value; }                                                                             \
            constexpr static std::array<std::string_view, size_type::value> arr() { return arr_##STRUCT_NAME; }                                      \
        };                                                                                                                                           \
        return reflect_members{};                                                                                                                    \
    }

#define MAKE_META_DATA(STRUCT_NAME, N, ...)                                                                                                          \
    constexpr inline std::array<std::string_view, N> arr_##STRUCT_NAME = {MAKE_STR_LIST(__VA_ARGS__)};                      \
    MAKE_META_DATA_IMPL(STRUCT_NAME, MAKE_ARG_LIST(N, &STRUCT_NAME::FIELD, __VA_ARGS__))

} // namespace iguana::detail

namespace iguana {
template <size_t I, typename T>
constexpr decltype(auto) get(T&& t);

template <typename T>
constexpr auto get(T const& t);
} // namespace iguana

namespace iguana::detail {
template <typename T, size_t... Is>
constexpr auto get_impl(T const& t, std::index_sequence<Is...>) {
    return std::make_tuple(iguana::get<Is>(t)...);
}

template <typename T, size_t... Is>
constexpr auto get_impl(T& t, std::index_sequence<Is...>) {
    return std::make_tuple(std::ref(get<Is>(t))...);
}
} // namespace iguana::detail

namespace iguana {
#define REFLECTION(STRUCT_NAME, ...) MAKE_META_DATA(STRUCT_NAME, GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)

#define __SEMICOLON__ ;
#define PARE(...) __VA_ARGS__
#define PAIR(x) PARE x // PAIR((int) x) => PARE(int) x => int x   

#define EAT(...)
#define STRIP(x) EAT x // STRIP((int) x) => EAT(int) x => x

#define DEFINE_STRUCT(st, ...)                                               \
    struct st {                                                              \
        FOR_EACH(PAIR, __SEMICOLON__, ##__VA_ARGS__);                        \
    };                                                                       \
    REFLECTION(st, FOR_EACH_COMMA(STRIP, __VA_ARGS__))

template <typename T>
using Reflect_members = decltype(iguana_reflect_members(std::declval<T>()));

template <typename T, typename = void>
struct is_reflection : std::false_type {};

template <typename T>
struct is_reflection<T, std::void_t<typename Reflect_members<T>::type>> : std::true_type {};

template <typename T>
inline constexpr bool is_reflection_v = is_reflection<T>::value;

//返回指定位置的值
template <size_t I, typename T>
constexpr decltype(auto) get(T&& t) {
    using M = decltype(iguana_reflect_members(std::forward<T>(t)));
    using U = decltype(std::forward<T>(t).*(std::get<I>(M::apply_impl())));

    if constexpr (std::is_array_v<U>) {
        auto s = std::forward<T>(t).*(std::get<I>(M::apply_impl()));
        std::array<char, sizeof(U)> arr;
        memcpy(arr.data(), s, arr.size());
        return arr;
    } else
        return std::forward<T>(t).*(std::get<I>(M::apply_impl()));
}

//返回一个包含struct里的值的tuple
template <typename T>
constexpr auto get(T const& t) {
    using M = decltype(iguana_reflect_members(t));
    return iguana::detail::get_impl(t, std::make_index_sequence<M::value()>{});
}

template <typename T>
constexpr auto get_ref(T& t) {
    using M = decltype(iguana_reflect_members(t));
    return iguana::detail::get_impl(t, std::make_index_sequence<M::value()>{});
}

//获取指定位置的字段的名字
template <typename T, size_t I>
constexpr const std::string_view get_name() {
    using M = decltype(iguana_reflect_members(std::declval<T>()));
    static_assert(I < M::value(), "out of range");
    return std::get<I>(M::arr());
}

//获取指定位置的字段的名字
template <typename T>
constexpr const std::string_view get_name(size_t i) {
    using M = decltype(iguana_reflect_members(std::declval<T>()));
    //		static_assert(I<M::value(), "out of range");
    return M::arr()[i];
}

//获取struct名称
template <typename T>
constexpr const std::string_view get_name() {
    using M = decltype(iguana_reflect_members(std::declval<T>()));
    return M::name();
}

//获取字段数量
template <typename T>
constexpr std::enable_if_t<is_reflection<T>::value, size_t> get_value() {
    using M = decltype(iguana_reflect_members(std::declval<T>()));
    return M::value();
}

template <typename T>
constexpr std::enable_if_t<!is_reflection<T>::value, size_t> get_value() {
    return 1;
}

//获取包含字段名的数组
template <typename T>
constexpr auto get_array() {
    using M = decltype(iguana_reflect_members(std::declval<T>()));
    return M::arr();
}

//根据字段名字获取所在的下标
template <typename T>
constexpr auto get_index(std::string_view name) {
    using M = decltype(iguana_reflect_members(std::declval<T>()));
    constexpr auto arr = M::arr();

    auto it = std::find_if(arr.begin(), arr.end(), [name](auto str) { return (str == name); });

    return std::distance(arr.begin(), it);
}

//指定tuple下标对应的值去执行function
template <class Tuple, class F, std::size_t... Is>
void tuple_switch(std::size_t i, Tuple&& t, F&& f, std::index_sequence<Is...>) {
    ((i == Is && ((std::forward<F>(f)(std::get<Is>(std::forward<Tuple>(t)))), false)), ...);
}

//-------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//
template <typename... Args, typename F, std::size_t... Idx>
constexpr void for_each(std::tuple<Args...>& t, F&& f, std::index_sequence<Idx...>) {
    (std::forward<F>(f)(std::get<Idx>(t), std::integral_constant<size_t, Idx>{}), ...);
}

//执行函数的具体实现
template <typename... Args, typename F, std::size_t... Idx>
constexpr void for_each(const std::tuple<Args...>& t, F&& f, std::index_sequence<Idx...>) {
    (std::forward<F>(f)(std::get<Idx>(t), std::integral_constant<size_t, Idx>{}), ...);
}

//遍历反射的对象，执行函数
template <typename T, typename F>
constexpr std::enable_if_t<is_reflection<T>::value> for_each(T&& t, F&& f) {
    using M = decltype(iguana_reflect_members(std::forward<T>(t)));
    for_each(M::apply_impl(), std::forward<F>(f), std::make_index_sequence<M::value()>{});
}

//遍历tuple元素，执行函数
template <typename T, typename F>
constexpr std::enable_if_t<is_tuple<std::decay_t<T>>::value> for_each(T&& t, F&& f) {
    // using M = decltype(iguana_reflect_members(std::forward<T>(t)));
    constexpr const size_t SIZE = std::tuple_size_v<std::decay_t<T>>;
    for_each(std::forward<T>(t), std::forward<F>(f), std::make_index_sequence<SIZE>{});
}
} // namespace iguana
#endif // IGUANA_REFLECTION_HPP
