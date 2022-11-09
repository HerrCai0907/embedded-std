/**
 * @File Name: fixed_list.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd__fixed_list__
#define __estd__fixed_list__

#include "array.h"
#include "type.h"
#include "type_traits.h"
#include "utility.h"

namespace estd {

namespace {
template <class T> class FixedListNode {
public:
  using value_type = T;
  T value_;
  mutable FixedListNode *prev_;
  mutable FixedListNode *post_;
};

template <class Ptr> class BaseIterator;
template <class T> class BaseIterator<T *> {
public:
  using value_type = T;
  using size_type = ::estd::size_t;
  using difference_type = ::estd::ptrdiff_t;
  using reference = value_type &;
  using pointer = value_type *;
  BaseIterator() : pointer_(nullptr) {}
  explicit BaseIterator(FixedListNode<value_type> *node_ptr) : pointer_(node_ptr) {}
  BaseIterator(BaseIterator<value_type const *> const &it) : pointer_(it.pointer_) {}

  reference operator*() const noexcept { return pointer_->value_; }
  pointer operator->() const noexcept { return &pointer_->value_; }
  BaseIterator &operator++() {
    pointer_ = pointer_->post_;
    return *this;
  }
  BaseIterator operator++(int) noexcept {
    BaseIterator old = *this;
    pointer_ = pointer_->post_;
    return old;
  }
  BaseIterator &operator--() {
    pointer_ = pointer_->prev_;
    return *this;
  }
  BaseIterator operator--(int) noexcept {
    BaseIterator old = *this;
    pointer_ = pointer_->prev_;
    return old;
  }
  bool operator==(BaseIterator other) const noexcept { return pointer_ == other.pointer_; }
  bool operator!=(BaseIterator other) const noexcept { return pointer_ != other.pointer_; }

private:
  template <class Type, ::estd::size_t N> friend class FixedListImpl;
  template <class Ptr> friend class BaseIterator;
  FixedListNode<value_type> *pointer_;
};

template <class T> class BaseIterator<T const *> {
public:
  using value_type = T;
  using size_type = ::estd::size_t;
  using difference_type = ::estd::ptrdiff_t;
  using reference = value_type &;
  using pointer = value_type *;
  BaseIterator() : pointer_(nullptr) {}
  explicit BaseIterator(FixedListNode<value_type> const *node_ptr)
      : pointer_(const_cast<FixedListNode<value_type> *>(node_ptr)) {}
  BaseIterator(BaseIterator<value_type *> const &it) : pointer_(it.pointer_) {}

  reference operator*() const noexcept { return pointer_->value_; }
  pointer operator->() const noexcept { return &pointer_->value_; }
  BaseIterator &operator++() {
    pointer_ = pointer_->post_;
    return *this;
  }
  BaseIterator operator++(int) noexcept {
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
  template <class Type, ::estd::size_t N> friend class FixedListImpl;
  template <class Ptr> friend class BaseIterator;
  FixedListNode<value_type> *pointer_;
};

template <class T, ::estd::size_t N> class FixedListImpl {
  static_assert(N != 0U, "size must more than zero");

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

  FixedListImpl(FixedListImpl const &other) noexcept = default;
  FixedListImpl(FixedListImpl &&other) noexcept = default;
  FixedListImpl &operator=(FixedListImpl const &other) noexcept = default;
  FixedListImpl &operator=(FixedListImpl &&other) noexcept = default;

  FixedListImpl() noexcept : size_(0U), storage_(), free_(&storage_[0U]) {
    init_free();
    storage_[N].prev_ = &storage_[N];
    storage_[N].post_ = &storage_[N];
  }
  FixedListImpl(size_type count) noexcept : FixedListImpl(count, T{}) {}
  FixedListImpl(size_type count, const T &value) noexcept : size_(count), storage_() {
    init_free();
    for (size_type i = 0U; i < size_; ++i) {
      storage_[i].value_ = value;
    }
    pre_alloc(size_);
  }
  template <class InputIt>
  FixedListImpl(InputIt first, InputIt last) noexcept : FixedListImpl(first, last, ::estd::is_integral<InputIt>{}) {}

  /**
   * @brief Returns a reference to the first element in the container. Calling front on an empty container is undefined.
   * @return reference: reference to the first element
   */
  reference front() noexcept { return storage_[N].post_->value_; }
  const_reference front() const noexcept { return storage_[N].post_->value_; }

  /**
   * @brief Returns a reference to the last element in the container. Calling back on an empty container causes
   * undefined behavior.
   * @return reference: Reference to the last element.
   */
  reference back() noexcept { return storage_[N].prev_->value_; }
  const_reference back() const noexcept { return storage_[N].prev_->value_; }

  iterator begin() noexcept { return iterator{storage_[N].post_}; }
  const_iterator begin() const noexcept { return const_iterator{storage_[N].post_}; }
  const_iterator cbegin() const noexcept { return begin(); }

  iterator end() noexcept { return iterator{&storage_[N]}; }
  const_iterator end() const noexcept { return const_iterator{&storage_[N]}; }
  const_iterator cend() const noexcept { return end(); }

  bool empty() const noexcept { return size_ == 0U; }
  size_type size() const noexcept { return size_; }
  size_type max_size() const noexcept { return N; }

  void clear() noexcept {
    for (iterator it = begin(); it != end(); ++it) {
      *it = T{};
    }
    init_free();
    storage_[N].post_ = &storage_[N];
    free_ = &storage_[0U];
    size_ = 0U;
  }

  iterator insert(const_iterator pos, const T &value) noexcept { return insert(pos, T{value}); }
  iterator insert(const_iterator pos, T &&value) noexcept {
    if (free_ == &storage_[N]) {
      return end();
    } else {
      free_->value_ = ::estd::move(value);
      FixedListNode<T> *current = free_;
      free_ = free_->post_;

      current->prev_ = pos.pointer_->prev_;
      pos.pointer_->prev_->post_ = current;
      current->post_ = pos.pointer_;
      pos.pointer_->prev_ = current;

      ++size_;
      return iterator{current};
    }
  }
  template <class InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last) noexcept {
    iterator first_it = pos;
    if (first != last) {
      first_it = insert(pos, *first);
      ++first;
      while (first != last) {
        insert(pos, *first);
        ++first;
      }
    }
    return first_it;
  }

  iterator erase(const_iterator pos) noexcept {
    FixedListNode<value_type> *position = pos.pointer_;
    position->prev_->post_ = position->post_;
    position->post_->prev_ = position->prev_;
    --size_;
    return iterator{position->post_};
  }
  iterator erase(const_iterator first, const_iterator last) noexcept {
    for (const_iterator it = first; it != last; ++it) {
      --size_;
    }
    FixedListNode<value_type> *first_position = first.pointer_;
    FixedListNode<value_type> *last_position = last.pointer_;
    first_position->prev_->post_ = last_position;
    last_position->prev_ = first_position->prev_;
    return iterator{last_position};
  }

  void push_back(const T &value) noexcept { insert(cend(), value); }
  void push_back(T &&value) noexcept { insert(cend(), ::estd::move(value)); }
  void push_front(const T &value) noexcept { insert(cbegin(), value); }
  void push_front(T &&value) noexcept { insert(cbegin(), ::estd::move(value)); }

private:
  template <class A, class B>
  FixedListImpl(A first, B last, ::estd::true_type) noexcept
      : FixedListImpl(static_cast<size_type>(first), static_cast<T>(last)) {}
  template <class InputIt>
  FixedListImpl(InputIt first, InputIt last, ::estd::false_type) noexcept : size_(0U), storage_(), free_() {
    init_free();
    for (InputIt it = first; it != last; ++it) {
      storage_[size_].value_ = *it;
      ++size_;
    }
    pre_alloc(size_);
  }
  void init_free() noexcept {
    FixedListNode<T> *current = &storage_[0U];
    for (size_type i = 0U; i < N; ++i) {
      current->post_ = &storage_[i + 1];
      current->prev_ = &storage_[i - 1];
      ++current;
    }
  }
  void pre_alloc(size_type size) noexcept {
    if (size_ > 0U) {
      storage_[size_ - 1U].post_ = &storage_[N];
      storage_[N].prev_ = &storage_[size_ - 1U];
      storage_[0U].prev_ = &storage_[N];
      storage_[N].post_ = &storage_[0U];
    } else {
      storage_[N].prev_ = &storage_[N];
      storage_[N].post_ = &storage_[N];
    }
    free_ = &storage_[size_];
  }

private:
  size_type size_;
  ::estd::Array<FixedListNode<T>, N + 1U> storage_;
  FixedListNode<T> *free_;
};

}; // namespace

template <class T, ::estd::size_t N> using FixedList = FixedListImpl<T, N>;

}; // namespace estd

#endif