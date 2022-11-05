/**
 * @File Name: type.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd_type__
#define __estd_type__

#ifndef __cpp_decltype
#error "not support decltype"
#endif

namespace estd {

/**
 * @brief std::size_t is the unsigned integer type of the result of the sizeof operator as well as the sizeof...
operator and the alignof operator (since C++11).
 */
using size_t = decltype(sizeof(int));

/**
 * @brief std::ptrdiff_t is the signed integer type of the result of subtracting two pointers.
 */
using ptrdiff_t = decltype(static_cast<char *>(nullptr) - static_cast<char *>(nullptr));

}; // namespace estd

#endif