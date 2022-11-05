/**
 * @File Name: algorithm.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd__algorithm__
#define __estd__algorithm__

#include "type.h"

namespace estd {

template <class T> void swap(T &a, T &b) noexcept {}

template <class T2, ::estd::size_t N> void swap(T2 (&a)[N], T2 (&b)[N]) noexcept {}

}; // namespace estd

#endif
