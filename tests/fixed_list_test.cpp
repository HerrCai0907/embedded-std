#include "fixed_list.h"
#include <gtest/gtest.h>
#include <list>
#include <vector>

std::list<int> a{};

TEST(FixedList, constructor) {
  ::estd::FixedList<int, 10> list{};
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.size(), 0);
}

TEST(FixedList, constructor_multiple) {
  ::estd::FixedList<int, 10> list(2);

  std::vector<int> res{};

  for (auto it = list.begin(); it != list.end(); ++it) {
    res.emplace_back(*it);
  }

  EXPECT_FALSE(list.empty());
  EXPECT_EQ(list.size(), 2);
  std::vector<int> expect_res(2);
  EXPECT_EQ(res, expect_res);
}

TEST(FixedList, constructor_multiple_value) {
  ::estd::FixedList<int, 10> list(2, 5);

  std::vector<int> res{};

  for (auto it = list.begin(); it != list.end(); ++it) {
    res.emplace_back(*it);
  }

  EXPECT_FALSE(list.empty());
  EXPECT_EQ(list.size(), 2);
  std::vector<int> expect_res{5, 5};
  EXPECT_EQ(res, expect_res);
}

TEST(FixedList, constructor_iterator) {
  std::vector<int> vec{1, 2, 3, 4};
  ::estd::FixedList<int, 10> list(vec.cbegin(), vec.cend());

  std::vector<int> res{};

  for (auto it = list.begin(); it != list.end(); ++it) {
    res.emplace_back(*it);
  }

  EXPECT_EQ(list.size(), vec.size());
  EXPECT_EQ(res, vec);
}