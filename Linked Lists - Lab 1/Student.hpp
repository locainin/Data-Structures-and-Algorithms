// Purpose: Declare Student class interface for the lab

#ifndef STUDENT_H
#define STUDENT_H
#include <iosfwd>
#include <string>

using std::ostream;
using std::string;

class Student {

private:
  // Student state used by linked-list demos
  string firstname; // first name
  string lastname;  // last name
  string BannerID;  // student id
  double gpa;            // grade point average

public:
  // construct with all fields
  Student(const string &firstName, const string &lastName, const string &bannerId,
          double gradePointAverage);
  [[nodiscard]] const string &getFirstname() const;
  [[nodiscard]] const string &getLastname() const;
  [[nodiscard]] const string &getBannerID() const;
  [[nodiscard]] double getGpa() const;
  void setFirstname(const string &firstName);
  void setLastname(const string &lastName);
  void setBannerID(const string &bannerId);
  void setGpa(double gradePointAverage);
  // Method to display the student's information
  void display() const;
  // Completed for you - Overload the << operator to print student details
  friend ostream &operator<<(ostream &outputStream, const Student &studentRecord);
};
#endif /* STUDENT_H */
