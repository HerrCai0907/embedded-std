#include "array.h"
#include <gtest/gtest.h>

struct ST {
  int a;
  int b;
  int c;
};

TEST(Array, fill) {
  const int VALUE = 11;
  ::estd::Array<int, 10> array{};
  array.fill(VALUE);

  for (auto it = array.cbegin(); it != array.cend(); it++) {
    EXPECT_EQ(*it, VALUE);
  }
}

TEST(Array, constructor) {
  ::estd::Array<int, 5> const array{1, 2, 3};
  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array.at(1), 2);
  EXPECT_EQ(array[2], 3);
}

TEST(Array, constructor_struct) {
  ST st1{};
  st1.a = 1;
  ST st2{};
  st2.a = 2;
  ::estd::Array<ST, 5> const array{ST{}, st1, std::move(st2)};
  EXPECT_EQ(array.at(1).a, 1);
  EXPECT_EQ(st1.a, 1);
  EXPECT_EQ(array[2].a, 2);
}

TEST(Array, to_array) {
  auto array = ::estd::to_array<int>(1, 2, 3);
  EXPECT_EQ(array.size(), 3);
  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array.at(1), 2);
  EXPECT_EQ(array[2], 3);
}

TEST(Array, to_array_struct) {
  ST st0{};
  st0.a = 1;
  ST st2{};
  st2.a = 2;
  auto array = ::estd::to_array(st0, ST{}, std::move(st2));
  EXPECT_EQ(array.at(0).a, 1);
  EXPECT_EQ(st0.a, 1);
  EXPECT_EQ(array[2].a, 2);
}

TEST(Array, swap) {
  auto arr1 = ::estd::to_array<int>(1, 2, 3);
  auto arr1_copy = arr1;
  auto arr2 = ::estd::to_array<int>(4, 5, 6);
  auto arr2_copy = arr2;
  std::swap(arr1, arr2);
  EXPECT_EQ(arr1, arr2_copy);
  EXPECT_EQ(arr2, arr1_copy);
}

TEST(ArrayDeathTest, at_out_of_range) {
  ::estd::Array<int, 5> const array{1, 2, 3};
  ASSERT_DEATH(array.at(5), "");
}