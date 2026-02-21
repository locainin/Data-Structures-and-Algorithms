// Demonstration: sorting algorithms (03_optimized_with_data_structure)
// Purpose: compare baseline and optimized behavior under the same workload

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kItemCount = 12000;

    std::mt19937 rng(46);
    std::uniform_int_distribution<int> dist(0, 1000000);

    std::vector<int> values;
    values.reserve(kItemCount);
    // Reuse baseline seed so both versions sort the same dataset
    for (int i = 0; i < kItemCount; i++) {
        values.push_back(dist(rng));
    }

    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();
    // Library sort provides optimized hybrid behavior
    std::sort(values.begin(), values.end());
    const auto end = std::chrono::steady_clock::now();

    // Convert wall-clock interval to integer milliseconds for report output
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Optimized Transaction Sorting\n";
    std::cout << "smallest: " << values.front() << " largest: " << values.back() << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
