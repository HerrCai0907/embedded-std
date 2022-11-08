/**
 * @File Name: utility.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd_utility__
#define __estd_utility__

#include "type_traits.h"

namespace estd {

template <class T> typename ::estd::remove_reference<T>::type &&move(T &t) noexcept {
  return (static_cast<typename ::estd::remove_reference<T>::type &&>(t));
}
template <class T> T &&forward(typename ::estd::remove_reference<T>::type &t) noexcept {
  return (static_cast<T &&>(t));
}
template <class T> T &&forward(typename ::estd::remove_reference<T>::type &&t) noexcept {
  static_assert(!::estd::is_lvalue_reference<T>::value, "cannot forward an rvalue as an lvalue");
  return (static_cast<T &&>(t));
}

}; // namespace estd

#endif
