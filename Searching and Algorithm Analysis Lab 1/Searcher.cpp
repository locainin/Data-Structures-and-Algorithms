// ##############################################
// #  Purpose: Provide custom linear and binary #
// #  search logic that tracks comparisons      #
// ##############################################

#include "Searcher.hpp"

// linear search through data
[[nodiscard]] SearchOutcome LinearSearch(const vector<int> &data, int target) {
  size_t comparisons = 0; // track comparisons made

  // go through each element
  for (size_t i = 0; i < data.size(); i++) {
    comparisons++; // count equality check

    // found target
    if (data[i] == target) {
      return {static_cast<ptrdiff_t>(i), comparisons};
    }
  }

  // not found
  return {-1, comparisons};
}

// binary search on sorted data
[[nodiscard]] SearchOutcome BinarySearch(const vector<int> &data, int target) {
  size_t comparisons = 0; // track comparisons
  ptrdiff_t low = 0;      // start index
  ptrdiff_t high = static_cast<ptrdiff_t>(data.size()) - 1; // end index

  // repeat until range is empty
  while (low <= high) {
    ptrdiff_t mid = low + (high - low) / 2; // middle index (avoid overflow)

    comparisons++; // check data[mid] == target
    if (data[static_cast<size_t>(mid)] == target) {
      return {mid, comparisons};
    }

    comparisons++; // choose side (< or >)
    if (data[static_cast<size_t>(mid)] < target) {
      low = mid + 1; // move right
    } else {
      high = mid - 1; // move left
    }
  }

  // not found
  return {-1, comparisons};
}
