// Purpose: Implement Student class member functions for the lab

#include "Student.hpp"
#include <iomanip>
#include <iostream>

using std::cout;
using std::fixed;
using std::ios;
using std::setprecision;
using std::streamsize;

// build a student with all fields set
Student::Student(const string &firstName, const string &lastName, const string &bannerId,
                 double gradePointAverage)
    : firstname(firstName), lastname(lastName), BannerID(bannerId), gpa(gradePointAverage) {}

// getters
const string &Student::getFirstname() const {
  return firstname; // read first name
}
const string &Student::getLastname() const {
  return lastname; // read last name
}
const string &Student::getBannerID() const {
  return BannerID; // read banner id
}
double Student::getGpa() const {
  return gpa; // read gpa
}

// setters
void Student::setFirstname(const string &firstName) {
  firstname = firstName; // set first name
}
void Student::setLastname(const string &lastName) {
  lastname = lastName; // set last name
}
void Student::setBannerID(const string &bannerId) {
  BannerID = bannerId; // set banner id
}
void Student::setGpa(double gradePointAverage) {
  gpa = gradePointAverage; // set gpa
}

// print current student with a newline
void Student::display() const {
  cout << *this << '\n'; // reuse stream operator
}

// Complete for you: Implementation of the << operator overload to print student details
// stream out one student on one line
ostream &operator<<(ostream &outputStream, const Student &studentRecord) {
  // preserve stream format settings used by callers
  const streamsize previousPrecision = outputStream.precision();
  const ios::fmtflags previousFlags = outputStream.flags();

  outputStream << "Name: " << studentRecord.firstname << " " << studentRecord.lastname << ", "
               << "BannerID: " << studentRecord.BannerID << ", "
               << "GPA: " << fixed << setprecision(2) << studentRecord.gpa; // format fields

  // restore stream format settings before return
  outputStream.flags(previousFlags);
  outputStream.precision(previousPrecision);
  return outputStream;                          // allow chaining
}
