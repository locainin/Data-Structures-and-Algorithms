// Demonstration: sorting algorithms (02_unoptimized)
// Purpose: compare baseline and optimized behavior under the same workload

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

namespace {

void bubbleSort(std::vector<int>& values) {
    const size_t n = values.size();
    // Outer pass bubbles the largest remaining value to the end
    for (size_t i = 0; i < n; i++) {
        bool swapped = false;
        // Inner pass compares adjacent values
        for (size_t j = 1; j < n - i; j++) {
            if (values[j - 1] > values[j]) {
                std::swap(values[j - 1], values[j]);
                swapped = true;
            }
        }
        if (!swapped) {
            return;
        }
    }
}

} // namespace

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kItemCount = 12000;

    std::mt19937 rng(46);
    std::uniform_int_distribution<int> dist(0, 1000000);

    std::vector<int> values;
    values.reserve(kItemCount);
    // Same random seed ensures repeatable benchmark inputs
    for (int i = 0; i < kItemCount; i++) {
        values.push_back(dist(rng));
    }

    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();
    bubbleSort(values);
    const auto end = std::chrono::steady_clock::now();

    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Unoptimized Transaction Sorting\n";
    std::cout << "smallest: " << values.front() << " largest: " << values.back() << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
