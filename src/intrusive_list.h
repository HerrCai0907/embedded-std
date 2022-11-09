#ifndef __estd_intrusive_list__
#define __estd_intrusive_list__

#include "abort.h"
#include "type.h"
#include "type_traits.h"

namespace estd {

template <class Node> class IntrusiveList;
namespace {
template <class Node> class BaseIterator;
};

template <class T, class GetNode, class GetElement> class IntrusiveListNode {
public:
  using value_type = T;
  using size_type = ::estd::size_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

  static IntrusiveListNode *get_node(T *const element) {
    static GetNode get_node_instance{};
    return get_node_instance(element);
  }
  static T *get_element(IntrusiveListNode *const node) {
    static GetElement get_element_instance{};
    return get_element_instance(node);
  }
  static T const *get_element(IntrusiveListNode const *const node) {
    static GetElement get_element_instance{};
    return get_element_instance(node);
  }

  IntrusiveListNode() noexcept : prev_{nullptr}, post_{nullptr} {}

  void link_self() noexcept {
    this->prev_ = this;
    this->post_ = this;
  }
  void insert_before(IntrusiveListNode *const insert_position) noexcept {
    IntrusiveListNode *const prev_position = insert_position->prev_;
    this->post_ = insert_position;
    this->prev_ = prev_position;
    insert_position->prev_ = this;
    prev_position->post_ = this;
  }
  void insert_after(IntrusiveListNode *const insert_position) noexcept {
    IntrusiveListNode *const post_position = insert_position->post_;
    this->post_ = post_position;
    this->prev_ = insert_position;
    insert_position->post_ = this;
    post_position->prev_ = this;
  }
  void erase() {
    this->post_->prev_ = this->prev_;
    this->prev_->post_ = this->post_;
  }

  size_type remove(const_reference value) noexcept {
    return remove_if([&value](const_reference element) { return element == value; });
  }

  template <class UnaryPredicate> size_type remove_if(UnaryPredicate p) noexcept {
    size_type remove_counter = 0U;
    IntrusiveListNode *current_node = this;
    IntrusiveListNode *end_node = this;
    bool find_end_node_flag = false;

    while (true) {
      IntrusiveListNode *next_node = current_node->post_;
      if (p(current_node->get_value())) {
        current_node->erase();
        ++remove_counter;
      } else {
        // find node not match request, it will be the end position of this remove
        break;
      }
      if (next_node == current_node) {
        // only one node
        break;
      }
      current_node = next_node;
    }
    end_node = current_node;
    while (current_node->post_ != end_node) {
      current_node = current_node->post_;
      if (p(current_node->get_value())) {
        current_node->erase();
        ++remove_counter;
      }
    }

    return remove_counter;
  }

  IntrusiveListNode *next() const noexcept { return post_; }
  reference get_value() noexcept { return *get_element(this); }

  size_type size() const noexcept {
    size_type list_size = 1;
    IntrusiveListNode *current = post_;
    while (current != this) {
      ++list_size;
      current = current->post_;
    }
    return list_size;
  }

protected:
  friend class IntrusiveList<IntrusiveListNode>;
  friend class BaseIterator<IntrusiveListNode>;
  friend class BaseIterator<IntrusiveListNode const>;
  IntrusiveListNode *prev_;
  IntrusiveListNode *post_;
};

namespace {

template <class Node> class BaseIterator {
public:
  using size_type = ::estd::size_t;
  using value_type = typename ::estd::conditional<::estd::is_const<Node>::value, const typename Node::value_type,
                                                  typename Node::value_type>::type;
  using pointer = value_type *;
  using reference = value_type &;

  explicit BaseIterator(Node *const node_p) : node_p_{node_p} {}
  BaseIterator &operator++() noexcept {
    node_p_ = node_p_->post_;
    return *this;
  }
  BaseIterator &operator++(int) noexcept {
    BaseIterator const old = *this;
    node_p_ = node_p_->post_;
    return old;
  }
  bool operator==(BaseIterator const &other) const noexcept { return node_p_ == other.node_p_; }
  bool operator!=(BaseIterator const &other) const noexcept { return node_p_ != other.node_p_; }

  reference operator*() const noexcept { return *Node::get_element(node_p_); }
  pointer operator->() const noexcept { return Node::get_element(node_p_); }

private:
  friend class IntrusiveList<Node>;
  Node *node_p_;
};

}; // namespace

template <class Node> class IntrusiveList {

public:
  using value_type = typename Node::value_type;
  using size_type = ::estd::size_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = BaseIterator<Node>;
  using const_iterator = BaseIterator<Node const>;

  IntrusiveList() noexcept : end_node_{}, size_{0U} {
    end_node_.post_ = &end_node_;
    end_node_.prev_ = &end_node_;
  }

  bool empty() const noexcept { return size_ == 0U; }
  size_type size() const noexcept { return size_; }

  iterator begin() noexcept { return iterator{end_node_.post_}; }
  iterator end() noexcept { return iterator{&end_node_}; }
  const_iterator begin() const noexcept { return const_iterator{end_node_.post_}; }
  const_iterator end() const noexcept { return const_iterator{&end_node_}; }

  /**
   * @brief inserts value before pos.
   * @param pos iterator before which the content will be inserted. pos may be the end() iterator
   * @param value element value to insert
   * @return iterator Iterator pointing to the inserted value.
   */
  iterator insert(iterator pos, reference value) noexcept {
    ++size_;
    Node *const node_p = Node::get_node(&value);
    node_p->insert_before(pos.node_p_);
    return iterator{node_p};
  }
  /**
   * @brief Removes the element at pos.
   * @param pos iterator to the element to remove
   * @return iterator Iterator following the last removed element.
   * @pre The iterator pos must be valid and dereferenceable.
   */
  iterator erase(iterator pos) noexcept {
    --size_;
    iterator const post_iter = iterator{pos.node_p_->post_};
    pos.node_p_->erase();
    return post_iter;
  }

  /**
   * @brief Appends the given element value to the end of the container.
   * @param value the value of the element to append
   */
  void push_back(reference value) noexcept {
    ++size_;
    Node *const node_p = Node::get_node(&value);
    node_p->insert_before(&end_node_);
  }
  /**
   * @brief Appends the given element value to the end of the container.
   * @param value the value of the element to append
   */
  void push_front(reference value) noexcept {
    ++size_;
    Node *const node_p = Node::get_node(&value);
    node_p->insert_before(end_node_.post_);
  }
  /**
   * @brief Removes the last element of the container. Calling pop_back on an empty container results in undefined
   * behavior. References and iterators to the erased element are invalidated.
   */
  void pop_back() noexcept {
    --size_;
    end_node_.prev_->erase();
  }
  /**
   * @brief Removes the first element of the container. If there are no elements in the container, the behavior is
   * undefined. References and iterators to the erased element are invalidated.
   */
  void pop_front() noexcept {
    --size_;
    end_node_.post_->erase();
  }

  size_type remove(const_reference value) noexcept {
    return remove_if([&value](const_reference element) { return element == value; });
  }

  template <class UnaryPredicate> size_type remove_if(UnaryPredicate p) noexcept {
    size_type old_size = size_;
    iterator it = begin();
    while (it != end()) {
      if (p(*it)) {
        it = erase(it);
      } else {
        ++it;
      }
    }
    return old_size - size_;
  }

private:
  Node end_node_{};
  size_type size_{};
};

template <class Node> bool operator==(IntrusiveList<Node> const &lhs, IntrusiveList<Node> const &rhs) noexcept {
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for (typename IntrusiveList<Node>::iterator lit = lhs.begin(), rit = rhs.begin(); lit != lhs.end(); ++lit, ++rit) {
    if (*lit != *rit) {
      return false;
    }
  }
  return true;
}
template <class Node> bool operator!=(IntrusiveList<Node> const &lhs, IntrusiveList<Node> const &rhs) noexcept {
  return !(lhs == rhs);
}

}; // namespace estd

#endif
