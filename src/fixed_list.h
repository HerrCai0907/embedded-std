/**
 * @File Name: fixed_list.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd__fixed_list__
#define __estd__fixed_list__

#include "array.h"
#include "type_traits.h"

namespace estd {

namespace {
template <class T> class FixedListNode {
public:
  using value_type = T;
  T value_;
  FixedListNode *prev_;
  FixedListNode *post_;
};

template <class Ptr> class BaseIterator;
template <class T> class BaseIterator<T *> {
public:
  using value_type = T;
  using size_type = ::estd::size_t;
  using difference_type = ::estd::ptrdiff_t;
  using reference = value_type &;
  BaseIterator() : pointer_(nullptr) {}
  BaseIterator(FixedListNode<T> *node_ptr) : pointer_(node_ptr) {}
  reference operator*() const noexcept { return pointer_->value_; }
  BaseIterator &operator++() {
    pointer_ = pointer_->post_;
    return *this;
  }
  BaseIterator &operator++(int) noexcept {
    BaseIterator old = *this;
    pointer_ = pointer_->post_;
    return old;
  }
  BaseIterator &operator--() {
    pointer_ = pointer_->prev_;
    return *this;
  }
  BaseIterator &operator--(int) noexcept {
    BaseIterator old = *this;
    pointer_ = pointer_->prev_;
    return old;
  }
  bool operator==(BaseIterator other) const noexcept { return pointer_ == other.pointer_; }
  bool operator!=(BaseIterator other) const noexcept { return pointer_ != other.pointer_; }

private:
  FixedListNode<T> *pointer_;
};

}; // namespace

template <class T, ::estd::size_t N> class FixedList {
  static_assert(N != 0, "size must more than zero");

public:
  using value_type = T;
  using size_type = ::estd::size_t;
  using difference_type = ::estd::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = BaseIterator<pointer>;
  using const_iterator = BaseIterator<const_pointer>;

  FixedList(FixedList const &other) noexcept = default;
  FixedList(FixedList &&other) noexcept = default;
  FixedList &operator=(FixedList const &other) noexcept = default;
  FixedList &operator=(FixedList &&other) noexcept = default;

  FixedList() noexcept : size_(0), storage_(), free_(&storage_[0]), head_(nullptr) { init_free(); }
  FixedList(size_type count) noexcept : FixedList(count, T{}) {}
  FixedList(size_type count, const T &value) noexcept
      : size_(count), storage_(), free_(&storage_[count]), head_(&storage_[0]) {
    init_free();
    for (size_type i = 0; i < size_; ++i) {
      storage_[i].value_ = value;
    }
    if (size_ > 0) {
      storage_[size_ - 1].post_ = &storage_[N];
      storage_[N].prev_ = &storage_[size_ - 1];
    }
  }
  template <class InputIt>
  FixedList(InputIt first, InputIt last) noexcept : FixedList(first, last, ::estd::is_integral<InputIt>{}) {}

  /**
   * @brief Returns a reference to the first element in the container. Calling front on an empty container is undefined.
   * @return reference: reference to the first element
   */
  reference front() noexcept { return head_->value_; }
  const_reference front() const noexcept { return head_->value_; }

  /**
   * @brief Returns a reference to the last element in the container. Calling back on an empty container causes
   * undefined behavior.
   * @return reference: Reference to the last element.
   */
  reference back() noexcept { return storage_[N].prev_->value_; }
  const_reference back() const noexcept { return storage_[N].prev_->value_; }

  iterator begin() noexcept { return iterator{&storage_[0]}; }
  const_iterator begin() const noexcept { iterator{&storage_[0]}; }
  const_iterator cbegin() const noexcept { iterator{&storage_[0]}; }

  iterator end() noexcept { return iterator{&storage_[N]}; }
  const_iterator end() const noexcept { return iterator{&storage_[N]}; }
  const_iterator cend() const noexcept { return iterator{&storage_[N]}; }

  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }
  size_type max_size() const noexcept { return N; }

  void clear() noexcept {}

private:
  template <class A, class B>
  FixedList(A first, B last, ::estd::true_type) noexcept
      : FixedList(static_cast<size_type>(first), static_cast<T>(last)) {}
  template <class InputIt>
  FixedList(InputIt first, InputIt last, ::estd::false_type) noexcept
      : size_(), storage_(), free_(), head_(&storage_[0]) {
    init_free();
    for (InputIt it = first; it != last; ++it) {
      storage_[size_].value_ = *it;
      ++size_;
    }
    if (size_ > 0) {
      storage_[size_ - 1].post_ = &storage_[N];
      storage_[N].prev_ = &storage_[size_ - 1];
    }
    free_ = &storage_[size_];
  }
  void init_free() noexcept {
    FixedListNode<T> *current = &storage_[0];
    for (size_type i = 0; i < N; ++i) {
      current->post_ = &storage_[i + 1];
      current->prev_ = &storage_[i - 1];
      ++current;
    }
  }

private:
  size_type size_;
  ::estd::Array<FixedListNode<T>, N + 1> storage_;
  FixedListNode<T> *free_;
  FixedListNode<T> *head_;
};

}; // namespace estd

#endif