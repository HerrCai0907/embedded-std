/**
 * @File Name: array.h
 * @author Congcong Cai (congcongcai0907@163.com)
 * @Creat Date : 2022-11-05
 * @copyright Copyright (c) {2022} Congcong Cai
 */

#ifndef __estd__array__
#define __estd__array__

#include "abort.h"
#include "algorithm.h"
#include "type.h"
#include "type_traits.h"
#include "utility.h"

namespace estd {

template <class T, ::estd::size_t N> class Array {
public:
  using value_type = T;
  using size_type = ::estd::size_t;
  using difference_type = ::estd::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = pointer;
  using const_iterator = const_pointer;

  Array() : data_{} {}
  template <class _Tp, class... _Args> Array(_Tp value, _Args... args) { init(0, value, args...); }

  /**
   * @brief access specified element
   * @param  pos: specified location pos
   * @return reference: specified element
   */
  reference operator[](size_type pos) noexcept { return data_[pos]; }
  const_reference operator[](size_type pos) const noexcept { return data_[pos]; }

  /**
   * @brief access specified element with bounds checking(abort)
   * @param  pos: specified location pos
   * @return reference: specified element
   */
  reference at(size_type pos) noexcept {
    this->check(pos);
    return data_[pos];
  }
  const_reference at(size_type pos) const noexcept {
    this->check(pos);
    return data_[pos];
  }

  /**
   * @brief access the first element
   * @return reference: first element
   */
  reference front() noexcept { return data_[0]; }
  const_reference front() const noexcept { return data_[0]; }

  /**
   * @brief access the last element
   * @return reference: last element
   */
  reference back() noexcept { return data_[N - 1]; }
  const_reference back() const noexcept { return data_[N - 1]; }

  /**
   * @brief direct access to the underlying array
   * @return pointer: underlying array pointer
   */
  pointer data() noexcept { return &data_[0]; }
  const_pointer data() const noexcept { return &data_[0]; }

  iterator begin() noexcept { return &data_[0]; }
  const_iterator begin() const noexcept { return &data_[0]; }
  const_iterator cbegin() const noexcept { return &data_[0]; }

  iterator end() noexcept { return &data_[N]; }
  const_iterator end() const noexcept { return &data_[N]; }
  const_iterator cend() const noexcept { return &data_[N]; }

  // reverse_iterator rbegin() noexcept;
  // const_reverse_iterator rbegin() const noexcept;
  // const_reverse_iterator crbegin() const noexcept;
  // reverse_iterator rend() noexcept;
  // const_reverse_iterator rend() const noexcept;
  // const_reverse_iterator crend() const noexcept;

  /**
   * @brief checks whether the container is empty
   * @return true: container is empty
   * @return false: container is not empty
   */
  bool empty() const noexcept { return N == 0; }
  /**
   * @brief returns the number of elements
   * @return size_type: number of elements
   */
  size_type size() const noexcept { return N; }
  /**
   * @brief returns the maximum possible number of elements
   * @return size_type: maximum possible number of elements
   */
  size_type max_size() const noexcept { return N; }

  /**
   * @brief Assigns the value to all elements in the container.
   * @param  value: the value to assign to the elements
   */
  void fill(const T &value) noexcept {
    for (iterator it = begin(); it != end(); ++it) {
      *it = value;
    }
  }

  /**
   * @brief Exchanges the contents of the container with those of other. Does not cause iterators and references to
   * associate with the other container.
   * @param  other: container to exchange the contents with
   */
  void swap(Array &other) noexcept {
    pointer temp = &data_[0];
    data_ = other.data_;
    other = temp;
  }

private:
  template <class _Tp, class... _Args> void init(size_type index, _Tp value, _Args... args) noexcept {
    static_assert(::estd::is_same<typename ::estd::remove_cvref<_Tp>::type, value_type>::value,
                  "initialized type does not match Array type");
    data_[index] = value;
    init(index + 1, args...);
  }
  template <class _Tp, class... _Args> void init(size_type index, _Tp value) noexcept { data_[index] = value; }

  void check(size_type pos) const noexcept {
    if (pos >= N) {
      abort();
    }
  }

private:
  value_type data_[N];
};

template <class T, ::estd::size_t N> bool operator==(const Array<T, N> &lhs, const Array<T, N> &rhs) noexcept {
  for (typename Array<T, N>::size_type i = 0; i < N; ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}
template <class T, ::estd::size_t N> bool operator!=(const Array<T, N> &lhs, const Array<T, N> &rhs) noexcept {
  return !(lhs == rhs);
}
template <class T, ::estd::size_t N> bool operator<(const Array<T, N> &lhs, const Array<T, N> &rhs) noexcept {
  for (typename Array<T, N>::size_type i = 0; i < N; ++i) {
    if (lhs[i] >= rhs[i]) {
      return false;
    }
  }
  return true;
}
template <class T, ::estd::size_t N> bool operator>(const Array<T, N> &lhs, const Array<T, N> &rhs) noexcept {
  for (typename Array<T, N>::size_type i = 0; i < N; ++i) {
    if (lhs[i] <= rhs[i]) {
      return false;
    }
  }
  return true;
}
template <class T, ::estd::size_t N> bool operator>=(const Array<T, N> &lhs, const Array<T, N> &rhs) noexcept {
  return !(lhs < rhs);
}
template <class T, ::estd::size_t N> bool operator<=(const Array<T, N> &lhs, const Array<T, N> &rhs) noexcept {
  return !(lhs > rhs);
}

template <::estd::size_t I, class T, ::estd::size_t N> T &get(Array<T, N> &a) noexcept {
  static_assert(I < N, "I must be an integer value in range [0, N).");
  return a[I];
}
template <::estd::size_t I, class T, ::estd::size_t N> T &&get(Array<T, N> &&a) noexcept {
  static_assert(I < N, "I must be an integer value in range [0, N).");
  return a[I];
}
template <::estd::size_t I, class T, ::estd::size_t N> T const &get(Array<T, N> const &a) noexcept {
  static_assert(I < N, "I must be an integer value in range [0, N).");
  return a[I];
}
template <::estd::size_t I, class T, ::estd::size_t N> T const &&get(Array<T, N> const &&a) noexcept {
  static_assert(I < N, "I must be an integer value in range [0, N).");
  return a[I];
}

template <class T, ::estd::size_t N> void swap(Array<T, N> &lhs, Array<T, N> &rhs) noexcept { lhs.swap(rhs); }

template <class V, class... T>
Array<typename ::estd::remove_reference<V>::type, sizeof...(T) + 1> to_array(V &&v, T &&...t) {
  return Array<typename ::estd::remove_reference<V>::type, sizeof...(T) + 1>{::estd::forward<V>(v),
                                                                             ::estd::forward<T>(t)...};
}

}; // namespace estd

#endif