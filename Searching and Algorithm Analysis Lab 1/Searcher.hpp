// Purpose: Define linear and binary search function declarations

#pragma once

#include <cstddef>
#include <vector>

using std::ptrdiff_t;
using std::size_t;
using std::vector;

// typed search result for better safety
struct SearchOutcome {
  ptrdiff_t index = -1;    // -1 means not found
  size_t comparisons = 0;  // number of key checks
};

// linear search on any data order
// returns index and comparison count
[[nodiscard]] SearchOutcome LinearSearch(const vector<int> &data, int target);

// binary search (loop form) on sorted data
// returns index and comparison count
[[nodiscard]] SearchOutcome BinarySearch(const vector<int> &data, int target);
