#include "fixed_list.h"
#include "type.h"
#include <gtest/gtest.h>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

template <class T, ::estd::size_t N>
bool operator==(::estd::FixedList<T, N> const &fixed_list, std::list<T> const &list) {
  if (list.size() != fixed_list.size()) {
    return false;
  }
  auto fixed_list_it = fixed_list.cbegin();
  auto list_it = list.cbegin();
  for (; fixed_list_it != fixed_list.cend() && list_it != list.cend(); fixed_list_it++, list_it++) {
    if (*fixed_list_it != *list_it) {
      return false;
    }
  }
  return true;
}

TEST(FixedList, constructor) {
  ::estd::FixedList<int, 10U> list{};
  EXPECT_TRUE(list.empty());
  EXPECT_TRUE(list.begin() == list.end());
  EXPECT_EQ(list.size(), 0U);
}

TEST(FixedList, constructor_multiple) {
  ::estd::FixedList<int, 10> list(2);
  std::list<int> expect(2);

  EXPECT_TRUE(list == expect);
}

TEST(FixedList, constructor_multiple_value) {
  ::estd::FixedList<int, 10> list(2, 5);
  std::list<int> expect(2, 5);

  EXPECT_TRUE(list == expect);
}

TEST(FixedList, constructor_iterator) {
  std::vector<int> vec{1, 2, 3, 4};
  ::estd::FixedList<int, 10> list(vec.cbegin(), vec.cend());
  std::list<int> expect(vec.cbegin(), vec.cend());

  EXPECT_TRUE(list == expect);
}

TEST(FixedList, clear) {
  std::vector<int> vec{1, 2, 3, 4};
  ::estd::FixedList<int, 10> list(vec.cbegin(), vec.cend());
  list.clear();

  std::list<int> expect{};
  EXPECT_TRUE(list == expect);
}

TEST(FixedList, insert) {
  std::vector<int> data{9, 7, 5, 2};

  ::estd::FixedList<int, 10> list{};
  std::list<int> expect{};

  for (int v : data) {
    auto it1 = list.insert(list.begin(), v);
    auto it2 = expect.insert(expect.begin(), v);
    EXPECT_TRUE(list == expect);
    EXPECT_EQ(*it1, *it2);
  }
}

TEST(FixedList, insert_range) {
  std::vector<int> data{9, 7, 5, 2};

  ::estd::FixedList<int, 10> list{};
  std::list<int> expect{};

  auto it1 = list.insert(list.begin(), data.cbegin(), data.cend());
  auto it2 = expect.insert(expect.cbegin(), data.cbegin(), data.cend());
  EXPECT_TRUE(list == expect);
  EXPECT_EQ(*it1, *it2);
}

TEST(FixedList, insert_null_range) {
  std::vector<int> data{};

  ::estd::FixedList<int, 10> list{};
  std::list<int> expect{};

  list.push_back(3);
  expect.push_back(3);

  auto it1 = list.insert(list.begin(), data.cbegin(), data.cend());
  auto it2 = expect.insert(expect.cbegin(), data.cbegin(), data.cend());
  EXPECT_TRUE(list == expect);
  EXPECT_EQ(*it1, *it2);
}

TEST(FixedList, earse) {
  std::vector<int> vec{1, 2, 3, 4, 5};
  ::estd::FixedList<int, 10> list(vec.cbegin(), vec.cend());
  std::list<int> expect(vec.cbegin(), vec.cend());
  {
    auto it1 = list.erase(list.begin());
    auto it2 = expect.erase(expect.begin());
    EXPECT_TRUE(list == expect);
    EXPECT_EQ(*it1, *it2);
  }
  {
    auto it1 = list.erase(--list.end());
    auto it2 = expect.erase(--expect.end());
    EXPECT_TRUE(list == expect);
    EXPECT_EQ(it1, list.end());
  }
  {
    auto it1 = list.erase(++list.begin());
    auto it2 = expect.erase(++expect.begin());
    EXPECT_TRUE(list == expect);
    EXPECT_EQ(*it1, *it2);
  }
}

TEST(FixedList, earse_range) {
  std::vector<int> vec{1, 2, 3, 4, 5};
  ::estd::FixedList<int, 10> list(vec.cbegin(), vec.cend());
  std::list<int> expect(vec.cbegin(), vec.cend());

  auto list_last = list.begin();
  ++list_last;
  ++list_last;
  auto it1 = list.erase(list.begin(), list_last);

  auto expect_last = expect.cbegin();
  ++expect_last;
  ++expect_last;
  auto it2 = expect.erase(expect.begin(), expect_last);

  EXPECT_TRUE(list == expect);
  EXPECT_EQ(*it1, *it2);
}

TEST(FixedList, push) {
  std::vector<int> data{7, 5, 3, 2};

  ::estd::FixedList<int, 10> list{};
  std::list<int> expect{};

  for (int v : data) {
    list.push_back(v);
    expect.push_back(v);
    EXPECT_TRUE(list == expect);
    list.push_front(v);
    expect.push_front(v);
    EXPECT_TRUE(list == expect);
  }
}
