// Purpose: Shared sort declarations and stats for the lab

#ifndef SORT_ALGORITHMS_HPP
#define SORT_ALGORITHMS_HPP

#include <cstddef>
#include <vector>

// selected std names used across sort files
using std::size_t;
using std::vector;

// simple operation counters for each run
struct SortStats {
  size_t comparisons = 0; // number of value comparisons
  size_t writes = 0;      // number of element writes
};

// in place sort implementations that also return operation stats
[[nodiscard]] SortStats bubbleSort(vector<int> &values);
[[nodiscard]] SortStats selectionSort(vector<int> &values);
[[nodiscard]] SortStats mergeSort(vector<int> &values);

// verify output is in nondecreasing order
[[nodiscard]] inline bool isNonDecreasing(const vector<int> &values) {
  if (values.size() < 2) {
    return true; // empty and single values are already sorted
  }
  for (size_t index = 1; index < values.size(); ++index) {
    if (values[index] < values[index - 1]) {
      return false; // found an inversion
    }
  }
  return true; // no inversions found
}

#endif
