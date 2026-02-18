// Purpose: Demonstrate singly linked list usage with integers strings and students

#include <iostream>
#include <string>

#include "SinglyLinkedList.hpp"
#include "Student.hpp"

using std::cout;
using std::string;

// shared banner text for section headers
constexpr const char sectionLine[] = "====================";

// helper to print a header line
void printSectionHeader(const string &title);

// print a consistent remove operation result
void printRemovalStatus(const string &valueLabel, bool wasRemoved);

int main() {
  // integer demo
  printSectionHeader("Integer List");
  SinglyLinkedList<int> integerList;
  integerList.insertAtHead(10);
  integerList.insertAtTail(20);
  integerList.insertAtTail(30);
  cout << "Initial: ";
  integerList.displayList();

  const bool removedTwenty = integerList.removeByValue(20);
  printRemovalStatus("20", removedTwenty);
  cout << "After remove 20: ";
  integerList.displayList();

  const bool removedNinetyNine = integerList.removeByValue(99);
  printRemovalStatus("99", removedNinetyNine);
  cout << "After remove 99: ";
  integerList.displayList();
  cout << "Size: " << integerList.size() << '\n';

  integerList.clear();
  cout << "After clear: ";
  integerList.displayList();

  const bool removedFromEmpty = integerList.removeByValue(10);
  printRemovalStatus("10 on empty list", removedFromEmpty);
  cout << "Size: " << integerList.size() << '\n';

  // string demo
  printSectionHeader("String List");
  SinglyLinkedList<string> stringList;
  stringList.insertAtHead("two");
  stringList.insertAtTail("three");
  stringList.insertAtHead("one");
  cout << "Initial: ";
  stringList.displayList();

  const bool removedThree = stringList.removeByValue("three");
  printRemovalStatus("three", removedThree);
  cout << "After remove three: ";
  stringList.displayList();
  cout << "Size: " << stringList.size() << '\n';

  stringList.clear();
  cout << "After clear: ";
  stringList.displayList();

  // student record demo
  printSectionHeader("Student List");
  SinglyLinkedList<Student> studentList;
  Student studentJohn("John", "Doe", "701123456", 3.95);
  Student studentJane("Jane", "Doe", "701234567", 3.85);
  Student studentBobStone("Bob", "Stone", "701345678", 3.45);
  Student studentClark("Clark", "Kent", "701456789", 3.70);
  studentList.insertAtHead(studentJohn);
  studentList.insertAtTail(studentJane);
  studentList.insertAtTail(studentBobStone);
  studentList.insertAtTail(studentClark);
  cout << "Roster:\n";
  studentList.displayListPerLine();
  cout << "Size: " << studentList.size() << '\n';

  studentList.clear();
  cout << "After clear: ";
  studentList.displayList();

  return 0;
}

// section header with a simple banner
void printSectionHeader(const string &title) {
  cout << '\n';
  cout << sectionLine << ' ' << title << ' ' << sectionLine << '\n';
}

void printRemovalStatus(const string &valueLabel, bool wasRemoved) {
  if (wasRemoved) {
    cout << "Remove " << valueLabel << ": removed\n";
  } else {
    cout << "Remove " << valueLabel << ": not found\n";
  }
}
