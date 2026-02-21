// Purpose: Selection sort implementation for the lab

#include "SortAlgorithms.hpp"

#include <utility>

// selected std name used in this file
using std::swap;

SortStats selectionSort(vector<int> &values) {
  SortStats stats; // tracks comparisons and writes
  if (values.size() < 2) {
    return stats; // already sorted
  }

  // grow a sorted prefix from left to right
  for (size_t sortedIndex = 0; sortedIndex + 1 < values.size(); sortedIndex++) {
    size_t smallestIndex = sortedIndex; // current best candidate

    // find smallest value in remaining unsorted range
    for (size_t scanIndex = sortedIndex + 1; scanIndex < values.size(); scanIndex++) {
      stats.comparisons++; // compare candidate values
      if (values[scanIndex] < values[smallestIndex]) {
        smallestIndex = scanIndex; // new minimum found
      }
    }

    // place smallest value at sortedIndex
    if (smallestIndex != sortedIndex) {
      swap(values[sortedIndex], values[smallestIndex]);
      stats.writes += 3; // model swap as 3 writes
    }
  }

  return stats;
}
