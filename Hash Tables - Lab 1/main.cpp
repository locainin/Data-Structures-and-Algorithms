// Purpose: run the hash table student record demo

#include "HashTable.hpp"

#include <iomanip>
#include <iostream>

using std::cout;
using std::left;
using std::setw;
using std::string;

void printSection(const string &title);
void printLookupResult(const HashTable &table, int idValue);

int main() {
  // Prime bucket count keeps distribution decent for small datasets
  HashTable table(13);

  printSection("Hash Table Demo");
  cout << "Bucket count: 13\n";

  // Demo data source uses "id name" rows
  const string fileName = "student_records.txt";
  loadFromFile(fileName, table);

  printSection("Search Checks");
  printLookupResult(table, 12345);
  printLookupResult(table, 67890);
  printLookupResult(table, 13579);

  printSection("Removal Check");
  table.remove(12345);
  printLookupResult(table, 12345);

  printSection("Bucket Dump");
  cout << "Chain format: (ID, Name) -> next\n";
  table.print();

  return 0;
}

void printSection(const string &title) {
  cout << "\n========================================\n";
  cout << title << '\n';
  cout << "========================================\n";
}

void printLookupResult(const HashTable &table, int idValue) {
  cout << "ID " << left << setw(7) << idValue << "=> " << table.search(idValue) << '\n';
}
