/**
 * @File Name: abort.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd__abort__
#define __estd__abort__

namespace estd {

inline void abort() { volatile int _ = *static_cast<volatile int *>(nullptr); }

}; // namespace estd

#endif