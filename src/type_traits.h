/**
 * @File Name: type_traits.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd__type_traits__
#define __estd__type_traits__

#include "type.h"

namespace estd {

template <class T, T v> struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; } // since c++14
};

using true_type = ::estd::integral_constant<bool, true>;
using false_type = ::estd::integral_constant<bool, false>;

template <class T> struct remove_cv { using type = T; };
template <class T> struct remove_cv<const T> { using type = T; };
template <class T> struct remove_cv<volatile T> { using type = T; };
template <class T> struct remove_cv<const volatile T> { using type = T; };

template <class T> struct remove_const { using type = T; };
template <class T> struct remove_const<const T> { using type = T; };

template <class T> struct remove_volatile { using type = T; };
template <class T> struct remove_volatile<volatile T> { using type = T; };

template <class T> struct remove_reference { using type = T; };
template <class T> struct remove_reference<T &> { using type = T; };
template <class T> struct remove_reference<T &&> { using type = T; };

template <class T> struct remove_cvref {
  using type = typename ::estd::remove_cv<typename ::estd::remove_reference<T>::type>::type;
};

template <class, class> struct is_same : ::estd::false_type {};
template <class T> struct is_same<T, T> : ::estd::true_type {};

template <class T> struct is_lvalue_reference : ::estd::false_type {};
template <class T> struct is_lvalue_reference<T &> : ::estd::true_type {};

template <class T> struct is_const : ::estd::false_type {};
template <class T> struct is_const<T const> : ::estd::true_type {};

template <bool B, class T, class F> struct conditional { using type = T; };
template <class T, class F> struct conditional<false, T, F> { using type = F; };

}; // namespace estd

#endif
