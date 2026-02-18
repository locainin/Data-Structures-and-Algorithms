#pragma once

#include <cstddef>
#include <string>
#include <vector>

using std::size_t;
using std::string;
using std::vector;

// Manages a student id and name
class Student {
public:
  Student(int idValue, const string &nameValue);

  [[nodiscard]] int getId() const;
  [[nodiscard]] const string &getName() const;
  void setName(const string &newName);

private:
  int id;
  string name;
};

// Node used for separate chaining
class HashNode {
public:
  HashNode(int studentId, const string &studentName, HashNode *nextNode);

  [[nodiscard]] Student &getStudent();
  [[nodiscard]] const Student &getStudent() const;
  void setNext(HashNode *nextNode);
  [[nodiscard]] HashNode *getNext() const;

private:
  Student student;
  HashNode *next;
};

// Hash table with separate chaining
class HashTable {
public:
  explicit HashTable(int size);
  ~HashTable();

  HashTable(const HashTable &) = delete;
  HashTable &operator=(const HashTable &) = delete;

  void insert(int key, const string &name);
  [[nodiscard]] string search(int key) const;
  void remove(int key);
  void print() const;

private:
  [[nodiscard]] size_t hashFunction(int key) const;
  void clearBucket(HashNode *node);

  size_t bucketCount;
  vector<HashNode *> buckets;
};

void loadFromFile(const string &fileName, HashTable &table);
