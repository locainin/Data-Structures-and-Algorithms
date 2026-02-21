#include "HashTable.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using std::cout;
using std::getline;
using std::ifstream;
using std::istringstream;
using std::ws;

// Load "id name" rows from text file into the table
// Invalid rows are ignored to keep the load resilient
void loadFromFile(const string &fileName, HashTable &table) {
  ifstream input(fileName);
  if (!input) {
    cout << "Could not open " << fileName << '\n';
    return;
  }

  string line;
  while (getline(input, line)) {
    // Skip blank lines early
    if (line.empty()) {
      continue;
    }

    istringstream stream(line);
    int id = 0;
    string name;

    // First token must be an integer id
    if (!(stream >> id)) {
      continue;
    }

    // Remaining content becomes the student name
    getline(stream >> ws, name);
    if (name.empty()) {
      continue;
    }

    table.insert(id, name);
  }
}

Student::Student(int idValue, const string &nameValue) : id(idValue), name(nameValue) {}

int Student::getId() const {
  return id;
}

const string &Student::getName() const {
  return name;
}

void Student::setName(const string &newName) {
  name = newName;
}

HashNode::HashNode(int studentId, const string &studentName, HashNode *nextNode)
    : student(studentId, studentName), next(nextNode) {}

Student &HashNode::getStudent() {
  return student;
}

const Student &HashNode::getStudent() const {
  return student;
}

void HashNode::setNext(HashNode *nextNode) {
  next = nextNode;
}

HashNode *HashNode::getNext() const {
  return next;
}

HashTable::HashTable(int size)
    : bucketCount(size > 0 ? static_cast<size_t>(size) : static_cast<size_t>(13)), buckets(bucketCount, nullptr) {}

// Free all chain nodes from every bucket
HashTable::~HashTable() {
  for (HashNode *node : buckets) {
    clearBucket(node);
  }
}

void HashTable::insert(int key, const string &name) {
  size_t index = hashFunction(key);
  HashNode *current = buckets[index];

  // Update existing record when ids match
  while (current != nullptr) {
    if (current->getStudent().getId() == key) {
      current->getStudent().setName(name);
      return;
    }
    current = current->getNext();
  }

  // Not found: push to head of this bucket chain
  HashNode *newNode = new HashNode(key, name, buckets[index]);
  buckets[index] = newNode;
}

string HashTable::search(int key) const {
  size_t index = hashFunction(key);
  HashNode *current = buckets[index];

  // Walk only one chain based on hash index
  while (current != nullptr) {
    if (current->getStudent().getId() == key) {
      return current->getStudent().getName();
    }
    current = current->getNext();
  }

  return "Not found";
}

void HashTable::remove(int key) {
  size_t index = hashFunction(key);
  HashNode *current = buckets[index];
  HashNode *previous = nullptr;

  // Remove first matching id from the chain
  while (current != nullptr) {
    if (current->getStudent().getId() == key) {
      if (previous == nullptr) {
        // Removing chain head
        buckets[index] = current->getNext();
      } else {
        // Bypass current node in the middle or tail
        previous->setNext(current->getNext());
      }
      delete current;
      return;
    }

    previous = current;
    current = current->getNext();
  }
}

void HashTable::print() const {
  for (size_t index = 0; index < buckets.size(); index++) {
    cout << "Bucket " << index << ": ";
    HashNode *current = buckets[index];

    if (current == nullptr) {
      cout << "empty\n";
      continue;
    }

    while (current != nullptr) {
      // Show chain nodes in traversal order
      cout << "(" << current->getStudent().getId() << ", " << current->getStudent().getName() << ")";
      current = current->getNext();
      if (current != nullptr) {
        cout << " -> ";
      }
    }
    cout << '\n';
  }
}

size_t HashTable::hashFunction(int key) const {
  // Keep negative keys in range [0, bucketCount)
  int modded = key % static_cast<int>(bucketCount);
  if (modded < 0) {
    modded += static_cast<int>(bucketCount);
  }
  return static_cast<size_t>(modded);
}

void HashTable::clearBucket(HashNode *node) {
  // Iterative chain delete avoids recursion depth issues
  while (node != nullptr) {
    HashNode *nextNode = node->getNext();
    delete node;
    node = nextNode;
  }
}
