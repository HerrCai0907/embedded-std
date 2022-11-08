#include "intrusive_list.h"
#include <gtest/gtest.h>
#include <list>

struct ST {
  struct GetNode;
  struct GetElement;

  struct GetNode {
    ::estd::IntrusiveListNode<ST, GetNode, GetElement> *operator()(ST *const element) noexcept {
      return &element->node_;
    }
    ::estd::IntrusiveListNode<ST, GetNode, GetElement> const *operator()(ST const *const element) noexcept {
      return &element->node_;
    }
  };
  struct GetElement {
    ST *operator()(::estd::IntrusiveListNode<ST, GetNode, GetElement> *const node) noexcept {
      return reinterpret_cast<ST *>(reinterpret_cast<char *>(node) - offsetof(ST, node_));
    }
    ST const *operator()(::estd::IntrusiveListNode<ST, GetNode, GetElement> const *const node) noexcept {
      return reinterpret_cast<ST const *>(reinterpret_cast<const char *>(node) - offsetof(ST, node_));
    }
  };
  ST() : node_{}, value_{0U} {}
  explicit ST(int v) : node_{}, value_{v} {}
  ::estd::IntrusiveListNode<ST, GetNode, GetElement> node_;
  int value_;
};

using STNode = ::estd::IntrusiveListNode<ST, ST::GetNode, ST::GetElement>;
using STList = ::estd::IntrusiveList<STNode>;

bool operator==(ST const &lhs, ST const &rhs) { return lhs.value_ == rhs.value_; }
bool operator!=(ST const &lhs, ST const &rhs) { return lhs.value_ != rhs.value_; }

bool operator==(STList const &lhs, std::list<ST> const &rhs) noexcept {
  if (lhs.size() != rhs.size()) {
    return false;
  }
  typename STList::const_iterator lit = lhs.begin();
  typename std::list<ST>::const_iterator rit = rhs.begin();
  for (; lit != lhs.end(); ++lit, ++rit) {
    if (*lit != *rit) {
      return false;
    }
  }
  return true;
}

TEST(IntrusiveList, insert) {
  STList list{};
  ST st1{2};
  auto it = list.insert(list.begin(), st1);
  EXPECT_EQ(&*it, &st1);
  EXPECT_EQ(list.begin()->value_, st1.value_);

  ST st2{3};
  it = list.insert(list.begin(), st2);
  EXPECT_EQ(&*it, &st2);
  EXPECT_EQ(list.begin()->value_, st2.value_);
}

TEST(IntrusiveList, erase) {
  STList list{};
  ST st1{2};
  auto it1 = list.insert(list.begin(), st1);
  ST st2{3};
  auto it2 = list.insert(list.begin(), st2);
  ST st3{5};
  auto it3 = list.insert(list.begin(), st3);

  list.erase(it2);

  std::list<ST> expect{st3, st1};

  EXPECT_EQ(list, expect);
}

TEST(IntrusiveList, push_and_pop) {
  STList list{};
  std::list<ST> expect{};
  ST st1{2};
  list.push_back(st1);
  expect.push_back(st1);
  EXPECT_EQ(list, expect);

  ST st2{3};
  list.push_back(st2);
  expect.push_back(st2);
  EXPECT_EQ(list, expect);

  ST st3{4};
  list.push_front(st3);
  expect.push_front(st3);
  EXPECT_EQ(list, expect);

  ST st4{5};
  list.push_front(st4);
  expect.push_front(st4);
  EXPECT_EQ(list, expect);

  list.pop_back();
  expect.pop_back();
  EXPECT_EQ(list, expect);

  list.pop_front();
  expect.pop_front();
  EXPECT_EQ(list, expect);

  list.pop_back();
  expect.pop_back();
  EXPECT_EQ(list, expect);

  list.pop_front();
  expect.pop_front();
  EXPECT_EQ(list, expect);
}

TEST(IntrusiveList, remove) {
  STList list{};
  std::list<ST> data{ST{1}, ST{2}, ST{3}, ST{4}, ST{5}, ST{6}};
  for (ST &st : data) {
    list.push_back(st);
  }
  list.remove(ST{2});
  data.remove(ST{2});

  EXPECT_EQ(list, data);
  EXPECT_EQ(list.size(), 5U);
}

TEST(IntrusiveList, remove_if) {
  STList list{};
  std::list<ST> data{ST{1}, ST{2}, ST{3}, ST{4}, ST{5}, ST{6}};
  for (ST &st : data) {
    list.push_back(st);
  }
  list.remove_if([](const ST &st) { return st.value_ % 2 == 1; });
  data.remove_if([](const ST &st) { return st.value_ % 2 == 1; });

  EXPECT_EQ(list, data);
  EXPECT_EQ(list.size(), 3U);
}
