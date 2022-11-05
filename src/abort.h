/**
 * @File Name: abort.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd__abort__
#define __estd__abort__

#include <cstdlib>
namespace estd {

#ifndef NO_STD_LIB
#include <stdlib.h>
inline void abort() { ::abort(); }
#else
inline void abort() { volatile int _ = *static_cast<volatile int *>(nullptr); }
#endif

}; // namespace estd

#endif