/*-------------------------------------------
 * Description: Simulates browser history
 * of domains using a stack and menu interface
 *-------------------------------------------*/

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <cctype>

using std::cin;
using std::cout;
using std::getline;
using std::isspace;
using std::queue;
using std::stack;
using std::string;
using std::stringstream;

// show main menu options
void showMenu();

// read and validate menu choice
[[nodiscard]] int readUserChoice();

// handle visiting a new page
void visitPage(stack<string> &history, queue<string> &visitLog);

// handle going back to previous page
void goBack(stack<string> &history);

// show the current top page
void showCurrentPage(const stack<string> &history);

// print all visited pages using a copy
void showAllPages(const stack<string> &history, const queue<string> &visitLog);

// print a queue from front to back using a copy
void printQueueFrontToBack(queue<string> queueCopy);

// helper to show what page user is on
void displayCurrentPage(const stack<string> &history);

// remove leading/trailing spaces from a string
[[nodiscard]] string trimWhitespace(const string &value);

// safely read user input and sanitize it
[[nodiscard]] bool readSanitizedLine(const string &promptMessage, string &sanitizedValue);

int main() {
  stack<string> history;      // active back-history with top as current page
  queue<string> visitLog;     // full visit order from oldest to newest
  bool keepRunning = true;    // controls main loop

  displayCurrentPage(history);

  while (keepRunning) {
    showMenu();
    int choice = readUserChoice();
    bool shouldDisplayCurrent = true;

    // handle closed input (EOF)
    if (choice == 0) {
      cout << "Input stream closed, exiting program\n";
      break;
    }

    // handle invalid or bad input
    if (choice == -1) {
      displayCurrentPage(history);
      continue;
    }

    // perform action based on user choice
    switch (choice) {
    case 1:
      visitPage(history, visitLog); // push page and log visit
      break;
    case 2:
      goBack(history); // pop last page
      break;
    case 3:
      showCurrentPage(history); // show top
      shouldDisplayCurrent = false;
      break;
    case 4:
      showAllPages(history, visitLog); // display stack + queue history
      break;
    case 5:
      cout << "Exiting program\n";
      keepRunning = false; // stop loop
      break;
    default:
      cout << "Please choose a valid option between 1 and 5\n";
      break;
    }

    // show current page after each valid action
    if (keepRunning && shouldDisplayCurrent) {
      displayCurrentPage(history);
    }
  }

  return 0;
}

// menu display for user choices
void showMenu() {
  cout << "\n--- Browser History Menu ---\n";
  cout << "1. Visit a new page\n";
  cout << "2. Go back\n";
  cout << "3. Show current page\n";
  cout << "4. Show browser history\n";
  cout << "5. Quit\n";
  cout << "Enter your choice: ";
}
// read the users menu choice safely
int readUserChoice() {
  string rawInput;
  if (!getline(cin, rawInput)) {
    return 0; // signal closed input
  }

  stringstream inputParser(rawInput);
  int choice = -1;
  char leftover = '\0';

  // make sure the user typed a single number only
  if (!(inputParser >> choice) || (inputParser >> leftover)) {
    cout << "Bad input, type a number between 1 and 5\n";
    return -1; // wrong input
  }

  return choice; // good input
}

// add a new site to the history
void visitPage(stack<string> &history, queue<string> &visitLog) {
  string siteName;

  // ask user for a website name
  if (!readSanitizedLine("Enter website name: ", siteName)) {
    return; // nothing entered
  }

  history.push(siteName);
  visitLog.push(siteName);
  cout << "Visited: " << siteName << '\n';
}

// go back one page if possible
void goBack(stack<string> &history) {
  if (history.empty()) {
    cout << "No pages to go back to\n";
    return;
  }

  if (history.size() == 1) {
    cout << "Only one page in history, can't go back\n";
    return;
  }

  string pageLeaving = history.top(); // page we're leaving
  history.pop();
  cout << "Going back from: " << pageLeaving << '\n';
}

// show stack top as the current page
void showCurrentPage(const stack<string> &history) {
  if (history.empty()) {
    cout << "Current page: No pages visited yet\n";
    return;
  }

  cout << "Current page: " << history.top() << '\n';
}

// copy stack to show all visited pages without modification
void showAllPages(const stack<string> &history, const queue<string> &visitLog) {
  if (history.empty() && visitLog.empty()) {
    cout << "History is empty\n";
    return;
  }

  // stack view shows active back-navigation order
  if (!history.empty()) {
    stack<string> historyCopy = history;
    cout << "Stack view (most recent to oldest):\n";
    while (!historyCopy.empty()) {
      cout << "- " << historyCopy.top() << '\n';
      historyCopy.pop();
    }
  } else {
    cout << "Stack view (most recent to oldest):\n";
    cout << "(empty)\n";
  }

  // queue view shows original visit order
  cout << "Queue view (oldest to newest):\n";
  printQueueFrontToBack(visitLog);
}

// print queue by value so original queue stays unchanged
void printQueueFrontToBack(queue<string> queueCopy) {
  if (queueCopy.empty()) {
    cout << "(empty)\n";
    return;
  }

  while (!queueCopy.empty()) {
    cout << "- " << queueCopy.front() << '\n';
    queueCopy.pop();
  }
}

// helper to show current page after each action
void displayCurrentPage(const stack<string> &history) {
  showCurrentPage(history);
}

// cut off spaces from start and end of text
string trimWhitespace(const string &value) {
  size_t first = 0;

  // move forward until a non-space is found
  while (first < value.size() && isspace(static_cast<unsigned char>(value[first]))) {
    first++;
  }

  // if all spaces, return empty string
  if (first == value.size()) {
    return "";
  }

  size_t last = value.size() - 1;

  // move backward until a non-space is found
  while (last > first && isspace(static_cast<unsigned char>(value[last]))) {
    last--;
  }

  // return text without outer spaces
  return value.substr(first, last - first + 1);
}

// get user text input and make sure it’s not empty
bool readSanitizedLine(const string &promptMessage, string &sanitizedValue) {
  cout << promptMessage;
  string rawInput;

  // read one full line of input
  if (!getline(cin, rawInput)) {
    cout << "Input closed before reading text\n";
    return false;
  }

  // remove spaces from both ends
  string cleaned = trimWhitespace(rawInput);

  // make sure something was typed
  if (cleaned.empty()) {
    cout << "Input cannot be empty\n";
    return false;
  }

  sanitizedValue = cleaned; // save clean text
  return true;              // input is good
}
