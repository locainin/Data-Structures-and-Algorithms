// Purpose: Compare sorting algorithms with clean educational output

#include "SortAlgorithms.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// selected std names used in this file
using std::cout;
using std::left;
using std::mt19937;
using std::size_t;
using std::sort;
using std::string;
using std::uniform_int_distribution;
using std::vector;
using std::setw;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

// per algorithm execution summary
struct AlgorithmResult {
  string algorithmName;               // display name in reports
  long long elapsedMicroseconds = 0;  // runtime in microseconds
  SortStats stats;                    // operation counters
  bool sortedOk = false;              // ascending order verification
};

// common function pointer type for sort implementations
using SortFunction = SortStats (*)(vector<int> &values);

// helper prototypes
[[nodiscard]] vector<int> buildInputValues(size_t valueCount, int minValue, int maxValue, unsigned int seed);
void printSection(const string &title);
void printSample(const vector<int> &values, size_t previewCount);
AlgorithmResult runAndPrintAlgorithm(const string &algorithmName, const string &lessonText,
                                     const vector<int> &inputValues, SortFunction sortFunction);
void printSummary(vector<AlgorithmResult> results);

int main() {
  // demo configuration
  const size_t valueCount = 150;
  const int minValue = 1;
  const int maxValue = 1000;
  const unsigned int randomSeed = 2026U;

  // single input reused by all algorithms for fairness
  const vector<int> inputValues = buildInputValues(valueCount, minValue, maxValue, randomSeed);

  // dataset metadata
  printSection("Sorting Algorithm Lab");
  cout << "Input size : " << inputValues.size() << '\n';
  cout << "Value range: [" << minValue << ", " << maxValue << "]\n";
  cout << "Seed       : " << randomSeed << " (fixed for repeatable demos)\n";

  // show a short input slice for context
  printSection("Input Preview");
  printSample(inputValues, 8);

  // run each algorithm on the same input copy
  printSection("Algorithm Runs");
  vector<AlgorithmResult> results;
  results.reserve(3);
  results.push_back(runAndPrintAlgorithm(
      "Bubble Sort", "Swaps adjacent pairs until no inversions remain", inputValues, bubbleSort));
  results.push_back(runAndPrintAlgorithm(
      "Selection Sort", "Selects the smallest remaining value for each position", inputValues, selectionSort));
  results.push_back(runAndPrintAlgorithm(
      "Merge Sort", "Splits input recursively and merges sorted halves", inputValues, mergeSort));

  printSummary(results);
  return 0;
}

vector<int> buildInputValues(size_t valueCount, int minValue, int maxValue, unsigned int seed) {
  mt19937 numberGenerator(seed);                                  // deterministic engine
  uniform_int_distribution<int> distribution(minValue, maxValue); // inclusive random range

  vector<int> values(valueCount); // output buffer
  for (size_t index = 0; index < values.size(); ++index) {
    values[index] = distribution(numberGenerator); // fill each position
  }
  return values;
}

void printSection(const string &title) {
  // consistent section break for readability
  cout << "\n==================== " << title << " ====================\n";
}

void printSample(const vector<int> &values, size_t previewCount) {
  if (values.empty()) {
    cout << "[empty]\n"; // no values to render
    return;
  }

  cout << '[';
  if (values.size() <= previewCount * 2) {
    // short input prints in full
    for (size_t index = 0; index < values.size(); ++index) {
      cout << values[index];
      if (index + 1 < values.size()) {
        cout << ", ";
      }
    }
  } else {
    // long input prints head and tail slices
    for (size_t index = 0; index < previewCount; ++index) {
      cout << values[index] << ", ";
    }

    cout << "..."; // hidden middle

    for (size_t index = values.size() - previewCount; index < values.size(); ++index) {
      cout << ", " << values[index];
    }
  }
  cout << "]\n";
}

AlgorithmResult runAndPrintAlgorithm(const string &algorithmName, const string &lessonText,
                                     const vector<int> &inputValues, SortFunction sortFunction) {
  vector<int> sortedValues = inputValues; // isolated copy for this algorithm

  // measured algorithm run
  const auto startTime = high_resolution_clock::now();
  const SortStats stats = sortFunction(sortedValues);
  const auto endTime = high_resolution_clock::now();

  const long long elapsedMicroseconds = duration_cast<microseconds>(endTime - startTime).count(); // elapsed time
  const bool sortedOk = isNonDecreasing(sortedValues); // correctness check

  // per algorithm educational output
  cout << '\n';
  cout << algorithmName << '\n';
  cout << "  Idea        : " << lessonText << '\n';
  cout << "  Output sample: ";
  printSample(sortedValues, 8);
  cout << "  Sorted check: " << (sortedOk ? "PASS" : "FAIL") << '\n';
  cout << "  Comparisons : " << stats.comparisons << '\n';
  cout << "  Writes      : " << stats.writes << '\n';
  cout << "  Time (us)   : " << elapsedMicroseconds << '\n';

  AlgorithmResult result; // packaged summary for final table
  result.algorithmName = algorithmName;
  result.elapsedMicroseconds = elapsedMicroseconds;
  result.stats = stats;
  result.sortedOk = sortedOk;
  return result;
}

void printSummary(vector<AlgorithmResult> results) {
  // sort by runtime so fastest algorithm appears first
  sort(results.begin(), results.end(),
       [](const AlgorithmResult &leftResult, const AlgorithmResult &rightResult) {
         return leftResult.elapsedMicroseconds < rightResult.elapsedMicroseconds;
       });

  // final comparison table
  printSection("Summary (Fastest First)");
  cout << left << setw(18) << "Algorithm" << setw(12) << "Time(us)" << setw(14) << "Comparisons" << setw(10)
       << "Writes" << "Sorted\n";
  cout << "---------------------------------------------------------------\n";

  for (const AlgorithmResult &result : results) {
    cout << left << setw(18) << result.algorithmName << setw(12) << result.elapsedMicroseconds << setw(14)
         << result.stats.comparisons << setw(10) << result.stats.writes
         << (result.sortedOk ? "PASS" : "FAIL") << '\n';
  }
}
