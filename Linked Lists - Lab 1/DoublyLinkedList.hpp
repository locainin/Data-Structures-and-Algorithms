// Purpose: small safe doubly linked list template with common operations
// notes: tries to avoid unsafe behavior, throws on empty front back and pops, many short comments

#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <stdexcept>
#include <vector>

using std::bidirectional_iterator_tag;
using std::move;
using std::out_of_range;
using std::ptrdiff_t;
using std::size_t;
using std::stable_sort;
using std::swap;
using std::vector;

template <typename T> class DoublyLinkedList {
private:
  // node stores value and links to neighbors
  struct Node {
    T value;    // stored value
    Node *prev; // back pointer
    Node *next; // forward pointer

    Node(const T &inputValue) : value(inputValue), prev(nullptr), next(nullptr) {}

    Node(T &&inputValue) : value(move(inputValue)), prev(nullptr), next(nullptr) {}
  };

public:
  class iterator {
  public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    // default null cursor
    iterator() : node(nullptr), owner(nullptr) {}

    // internal constructor used by list
    iterator(Node *inputNode, DoublyLinkedList *inputOwner) : node(inputNode), owner(inputOwner) {}

    // dereference to value
    reference operator*() const { return node->value; }

    // arrow to value
    pointer operator->() const { return &node->value; }

    // move forward
    iterator &operator++() {
      if (node) {
        node = node->next;
      } else {
        node = nullptr;
      }
      return *this;
    }

    // return old then move forward
    iterator operator++(int) {
      iterator temp(*this);
      if (node) {
        node = node->next;
      } else {
        node = nullptr;
      }
      return temp;
    }

    // move backward or from end to last
    iterator &operator--() {
      if (node) {
        node = node->prev; // move to previous
      } else if (owner) {
        node = owner->tail; // from end() step to last
      }
      return *this;
    }

    // return old then move backward
    iterator operator--(int) {
      iterator temp(*this);
      if (node) {
        node = node->prev;
      } else if (owner) {
        node = owner->tail;
      }
      return temp;
    }

    // compare positions
    friend bool operator==(const iterator &left, const iterator &right) { return left.node == right.node; }

    // compare positions
    friend bool operator!=(const iterator &left, const iterator &right) { return left.node != right.node; }

  private:
    Node *node;              // current node or nullptr for end
    DoublyLinkedList *owner; // used so step back from end goes to last

    friend class DoublyLinkedList;
    friend class const_iterator;
  };

  class const_iterator {
  public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    // default null const cursor
    const_iterator() : node(nullptr), owner(nullptr) {}

    // internal constructor used by list
    const_iterator(Node *inputNode, const DoublyLinkedList *inputOwner)
        : node(inputNode), owner(inputOwner) {}

    // convert from non const keeps position and owner
    const_iterator(const iterator &other) : node(other.node), owner(other.owner) {}

    // dereference to const value
    reference operator*() const { return node->value; }

    // arrow to const value
    pointer operator->() const { return &node->value; }

    // move forward
    const_iterator &operator++() {
      if (node) {
        node = node->next;
      } else {
        node = nullptr;
      }
      return *this;
    }

    // return old then move forward
    const_iterator operator++(int) {
      const_iterator temp(*this);
      if (node) {
        node = node->next;
      } else {
        node = nullptr;
      }
      return temp;
    }

    // move backward or from end to last
    const_iterator &operator--() {
      if (node) {
        node = node->prev;
      } else if (owner) {
        node = owner->tail;
      }
      return *this;
    }

    // return old then move backward
    const_iterator operator--(int) {
      const_iterator temp(*this);
      if (node) {
        node = node->prev;
      } else if (owner) {
        node = owner->tail;
      }
      return temp;
    }

    // compare positions
    friend bool operator==(const const_iterator &left, const const_iterator &right) {
      return left.node == right.node;
    }

    // compare positions
    friend bool operator!=(const const_iterator &left, const const_iterator &right) {
      return left.node != right.node;
    }

  private:
    Node *node;                    // current node or nullptr for end
    const DoublyLinkedList *owner; // used so step back from end goes to last

    friend class DoublyLinkedList;
  };

  // default construct empty list
  DoublyLinkedList() : head(nullptr), tail(nullptr), nodeCount(0) {}

  // copy construct by pushing in order
  DoublyLinkedList(const DoublyLinkedList &other) : DoublyLinkedList() {
    for (const T &value : other) {
      push_back(value);
    }
  }

  // move construct steals links
  DoublyLinkedList(DoublyLinkedList &&other) noexcept
      : head(other.head), tail(other.tail), nodeCount(other.nodeCount) {
    other.head = nullptr; // leave other empty
    other.tail = nullptr;
    other.nodeCount = 0;
  }

  // destructor clears all nodes
  ~DoublyLinkedList() { clear(); }

  // copy swap assignment
  DoublyLinkedList &operator=(DoublyLinkedList other) noexcept {
    swapWith(other);
    return *this;
  }

  // swap internals
  void swapWith(DoublyLinkedList &other) noexcept {
    swap(head, other.head);
    swap(tail, other.tail);
    swap(nodeCount, other.nodeCount);
  }

  // list state queries
  [[nodiscard]] bool empty() const noexcept { return nodeCount == 0; }

  [[nodiscard]] size_t size() const noexcept { return nodeCount; }

  // begin and end attach owner so step back from end goes to last
  iterator begin() noexcept { return iterator(head, this); }

  const_iterator begin() const noexcept { return const_iterator(head, this); }

  const_iterator cbegin() const noexcept { return const_iterator(head, this); }

  iterator end() noexcept { return iterator(nullptr, this); }

  const_iterator end() const noexcept { return const_iterator(nullptr, this); }

  const_iterator cend() const noexcept { return const_iterator(nullptr, this); }

  // access first value, throws if empty
  T &front() {
    if (!head)
      throw out_of_range("front on empty list");
    return head->value;
  }

  // const access first, throws if empty
  const T &front() const {
    if (!head)
      throw out_of_range("front on empty list");
    return head->value;
  }

  // access last value, throws if empty
  T &back() {
    if (!tail)
      throw out_of_range("back on empty list");
    return tail->value;
  }

  // const access last, throws if empty
  const T &back() const {
    if (!tail)
      throw out_of_range("back on empty list");
    return tail->value;
  }

  // remove all nodes
  void clear() noexcept {
    Node *current = head;
    while (current) {
      Node *nextNode = current->next; // stash next
      delete current;                 // free node
      current = nextNode;             // advance
    }
    head = nullptr;
    tail = nullptr;
    nodeCount = 0;
  }

  // push at front by inserting before head
  void push_front(const T &value) { insert(begin(), value); }

  // push front using move
  void push_front(T &&value) { insert(begin(), move(value)); }

  // push at back by inserting at end
  void push_back(const T &value) { insert(end(), value); }

  // push back using move
  void push_back(T &&value) { insert(end(), move(value)); }

  // remove first element
  void pop_front() {
    if (!head) {
      throw out_of_range("pop_front on empty list");
    }
    erase(begin()); // erase first
  }

  // remove last element
  void pop_back() {
    if (!tail) {
      throw out_of_range("pop_back on empty list");
    }
    Node *last = tail; // save last
    if (last->prev) {
      last->prev->next = nullptr; // unlink from previous
    } else {
      head = nullptr; // list becomes empty
    }
    tail = last->prev; // move tail back
    delete last;       // free removed node
    nodeCount--;       // count down
  }

  // insert value before position and return new position
  iterator insert(iterator position, const T &value) {
    Node *positionNode = position.node; // target node or null for end
    Node *newNode = new Node(value);    // allocate new node
    linkBefore(newNode, positionNode);  // link into list
    return iterator(newNode, this);     // point to inserted
  }

  // insert before position using move
  iterator insert(iterator position, T &&value) {
    Node *positionNode = position.node;
    Node *newNode = new Node(move(value));
    linkBefore(newNode, positionNode);
    return iterator(newNode, this);
  }

  // erase element at position and return next position
  iterator erase(iterator position) {
    Node *targetNode = position.node; // node to erase
    if (!targetNode) {
      return end(); // nothing to erase
    }
    Node *nextNode = targetNode->next; // stash next
    if (targetNode->prev) {
      targetNode->prev->next = targetNode->next; // bypass target
    } else {
      head = targetNode->next; // moved head
    }
    if (targetNode->next) {
      targetNode->next->prev = targetNode->prev; // link back
    } else {
      tail = targetNode->prev; // moved tail
    }
    delete targetNode;               // free
    nodeCount--;                     // count down
    return iterator(nextNode, this); // point to successor
  }

  // remove all elements that match test and return how many were removed
  template <typename Predicate> size_t remove_if(Predicate predicate) {
    size_t removedCount = 0;    // number removed
    iterator current = begin(); // walk from head
    while (current != end()) {
      if (predicate(*current)) {
        current = erase(current); // erase returns next
        removedCount++;
      } else {
        current++;
      }
    }
    return removedCount;
  }

  // reverse list by swapping links
  void reverse() noexcept {
    Node *current = head; // start at head
    while (current) {
      Node *nextNode = current->next; // save next
      current->next = current->prev;  // flip next
      current->prev = nextNode;       // flip prev
      current = nextNode;             // advance via saved next
    }
    swap(head, tail); // head and tail swap
  }

  // sort by copying values to a buffer then write back
  template <typename Compare> void sort(Compare compare) {
    if (nodeCount < 2) {
      return; // already sorted
    }
    vector<T> buffer; // temp storage
    buffer.reserve(nodeCount);
    for (const T &value : *this) {
      buffer.push_back(value); // copy values
    }
    stable_sort(buffer.begin(), buffer.end(), compare); // stable order
    iterator dataIterator = begin();
    for (const T &value : buffer) {
      *dataIterator = value; // write back in order
      dataIterator++;
    }
  }

  // default sort uses less than
  void sort() {
    sort([](const T &leftValue, const T &rightValue) { return leftValue < rightValue; });
  }

private:
  Node *head;             // first node or null
  Node *tail;             // last node or null
  size_t nodeCount; // number of nodes

  // link node before position, position may be null which means push back
  void linkBefore(Node *node, Node *position) {
    if (!position) { // append at end
      node->prev = tail;
      node->next = nullptr;
      if (tail) {
        tail->next = node; // stitch after current tail
      } else {
        head = node; // first element
      }
      tail = node; // new tail
    } else {       // insert before position
      node->prev = position->prev;
      node->next = position;
      if (position->prev) {
        position->prev->next = node; // stitch previous to new
      } else {
        head = node; // new head
      }
      position->prev = node; // stitch position back
    }
    nodeCount++; // count up
  }
};

template <typename T> void swap(DoublyLinkedList<T> &leftList, DoublyLinkedList<T> &rightList) noexcept {
  leftList.swapWith(rightList); // swap helper
}

#endif
