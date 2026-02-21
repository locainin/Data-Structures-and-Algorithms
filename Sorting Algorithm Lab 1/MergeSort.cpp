// Purpose: Merge sort implementation for the lab

#include "SortAlgorithms.hpp"

namespace {

// merge two sorted ranges [left..mid] and [mid+1..right]
void mergeHalves(vector<int> &values, vector<int> &tempSpace, size_t leftIndex, size_t middleIndex,
                 size_t rightIndex, SortStats &stats);

// recursively split range and merge it back sorted
void mergeSortRecursive(vector<int> &values, vector<int> &tempSpace, size_t leftIndex, size_t rightIndex,
                        SortStats &stats);

void mergeSortRecursive(vector<int> &values, vector<int> &tempSpace, size_t leftIndex, size_t rightIndex,
                        SortStats &stats) {
  if (leftIndex >= rightIndex) {
    return; // base case with one value
  }

  const size_t middleIndex = leftIndex + (rightIndex - leftIndex) / 2; // overflow safe midpoint
  mergeSortRecursive(values, tempSpace, leftIndex, middleIndex, stats); // sort left half
  mergeSortRecursive(values, tempSpace, middleIndex + 1, rightIndex, stats); // sort right half
  mergeHalves(values, tempSpace, leftIndex, middleIndex, rightIndex, stats); // combine halves
}

void mergeHalves(vector<int> &values, vector<int> &tempSpace, size_t leftIndex, size_t middleIndex,
                 size_t rightIndex, SortStats &stats) {
  size_t leftPosition = leftIndex;        // read cursor in left half
  size_t rightPosition = middleIndex + 1; // read cursor in right half
  size_t writePosition = leftIndex;       // write cursor in temp buffer

  // take smaller front value from the two halves
  while (leftPosition <= middleIndex && rightPosition <= rightIndex) {
    stats.comparisons++; // compare both front values
    if (values[leftPosition] <= values[rightPosition]) {
      tempSpace[writePosition++] = values[leftPosition++]; // consume left value
    } else {
      tempSpace[writePosition++] = values[rightPosition++]; // consume right value
    }
    stats.writes++; // track buffer write
  }

  // copy leftover items from left half
  while (leftPosition <= middleIndex) {
    tempSpace[writePosition++] = values[leftPosition++];
    stats.writes++;
  }

  // copy leftover items from right half
  while (rightPosition <= rightIndex) {
    tempSpace[writePosition++] = values[rightPosition++];
    stats.writes++;
  }

  // copy merged result back to source vector
  for (size_t assignIndex = leftIndex; assignIndex <= rightIndex; assignIndex++) {
    values[assignIndex] = tempSpace[assignIndex];
    stats.writes++;
  }
}

} // namespace

SortStats mergeSort(vector<int> &values) {
  SortStats stats; // tracks comparisons and writes
  if (values.size() < 2) {
    return stats; // already sorted
  }

  vector<int> tempSpace(values.size()); // one reusable buffer for all merges
  mergeSortRecursive(values, tempSpace, 0, values.size() - 1, stats);
  return stats;
}
