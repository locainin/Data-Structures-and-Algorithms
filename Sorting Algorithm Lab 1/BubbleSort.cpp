// Purpose: Bubble sort implementation for the lab

#include "SortAlgorithms.hpp"

#include <utility>

using std::swap;

SortStats bubbleSort(vector<int> &values) {
  SortStats stats; // tracks comparisons and writes
  if (values.size() < 2) {
    return stats; // already sorted
  }

  size_t unsortedEnd = values.size() - 1; // tail boundary for this pass
  bool swapped = true;                    // early stop when no swaps happen

  // Keep iterating until a full pass performs zero swaps
  while (swapped) {
    swapped = false; // assume sorted until a swap is seen
    // Traverse the unsorted prefix and compare adjacent values
    for (size_t index = 0; index < unsortedEnd; index++) {
      const size_t nextIndex = index + 1; // adjacent right element
      stats.comparisons++;                // compare neighbors
      if (values[index] > values[nextIndex]) {
        swap(values[index], values[nextIndex]); // push larger value right
        stats.writes += 3;                      // model swap as 3 writes
        swapped = true;                         // keep iterating
      }
    }
    if (unsortedEnd == 0) {
      break; // no more work left
    }
    unsortedEnd--; // last element is now fixed
  }

  return stats;
}
