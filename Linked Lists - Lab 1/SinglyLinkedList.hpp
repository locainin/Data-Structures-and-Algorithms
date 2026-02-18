// Purpose: Define a safe singly linked list template for lab demos

#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include <cstddef>
#include <iostream>
#include <utility>

using std::cout;
using std::move;
using std::size_t;
using std::swap;

// node for single linked list
template <typename T> class Node {
public:
  T data;     // value in this node
  Node *next; // pointer to next node
  // build node from lvalue
  explicit Node(const T &value) : data(value), next(nullptr) {}
  // build node from rvalue
  explicit Node(T &&value) : data(move(value)), next(nullptr) {}
};

// simple singly linked list
template <typename T> class SinglyLinkedList {
private:
  Node<T> *head;   // first node in the list
  size_t listSize; // tracked node count

  // copy nodes from another list
  void copyFrom(const SinglyLinkedList &other) {
    Node<T> *currentNode = other.head;
    while (currentNode != nullptr) {
      insertAtTail(currentNode->data);
      currentNode = currentNode->next;
    }
  }

  // swap internal pointers and size
  void swapWith(SinglyLinkedList &other) noexcept {
    swap(head, other.head);
    swap(listSize, other.listSize);
  }

public:
  // start with an empty list
  SinglyLinkedList() : head(nullptr), listSize(0) {}

  // deep copy list nodes
  SinglyLinkedList(const SinglyLinkedList &other) : head(nullptr), listSize(0) { copyFrom(other); }

  // move list ownership
  SinglyLinkedList(SinglyLinkedList &&other) noexcept : head(other.head), listSize(other.listSize) {
    other.head = nullptr;
    other.listSize = 0;
  }

  // free all nodes on destroy
  ~SinglyLinkedList() { clear(); }

  // copy assignment with strong exception safety
  SinglyLinkedList &operator=(SinglyLinkedList other) noexcept {
    swapWith(other);
    return *this;
  }

  // quick state checks
  [[nodiscard]] bool empty() const noexcept { return listSize == 0; }
  [[nodiscard]] size_t size() const noexcept { return listSize; }

  // put a new node at the front
  void insertAtHead(const T &value) {
    Node<T> *newNode = new Node<T>(value);
    newNode->next = head;
    head = newNode;
    listSize++;
  }

  // put a moved value at the front
  void insertAtHead(T &&value) {
    Node<T> *newNode = new Node<T>(move(value));
    newNode->next = head;
    head = newNode;
    listSize++;
  }

  // put a new node at the end
  void insertAtTail(const T &value) {
    Node<T> *newNode = new Node<T>(value);
    if (!head) {
      head = newNode;
      listSize++;
      return;
    }
    Node<T> *currentNode = head;
    while (currentNode->next != nullptr) {
      currentNode = currentNode->next;
    }
    currentNode->next = newNode;
    listSize++;
  }

  // put a moved value at the end
  void insertAtTail(T &&value) {
    Node<T> *newNode = new Node<T>(move(value));
    if (!head) {
      head = newNode;
      listSize++;
      return;
    }
    Node<T> *currentNode = head;
    while (currentNode->next != nullptr) {
      currentNode = currentNode->next;
    }
    currentNode->next = newNode;
    listSize++;
  }

  // remove first matching value and report success
  [[nodiscard]] bool removeByValue(const T &value) {
    if (!head) {
      return false;
    }

    // remove at head when matched
    if (head->data == value) {
      Node<T> *nodeToDelete = head;
      head = head->next;
      delete nodeToDelete;
      listSize--;
      return true;
    }

    // search from the second node
    Node<T> *currentNode = head;
    while (currentNode->next != nullptr && currentNode->next->data != value) {
      currentNode = currentNode->next;
    }

    // unlink when found
    if (currentNode->next != nullptr) {
      Node<T> *nodeToDelete = currentNode->next;
      currentNode->next = nodeToDelete->next;
      delete nodeToDelete;
      listSize--;
      return true;
    } else {
      return false;
    }
  }

  // print values in a chain format
  void displayList() const {
    cout << '[';
    Node<T> *currentNode = head;
    bool isFirstValue = true;
    while (currentNode != nullptr) {
      if (!isFirstValue) {
        cout << " -> ";
      }
      cout << currentNode->data;
      isFirstValue = false;
      currentNode = currentNode->next;
    }
    cout << "]\n";
  }

  // print one value per line with an index
  void displayListPerLine() const {
    Node<T> *currentNode = head;
    size_t index = 1;
    while (currentNode != nullptr) {
      cout << "  [" << index << "] " << currentNode->data << '\n';
      currentNode = currentNode->next;
      index++;
    }
  }

  // delete all nodes and reset list
  void clear() {
    Node<T> *currentNode = head;
    while (currentNode != nullptr) {
      Node<T> *nodeToDelete = currentNode;
      currentNode = currentNode->next;
      delete nodeToDelete;
    }
    head = nullptr;
    listSize = 0;
  }
};

#endif /* SINGLYLINKEDLIST_H */
