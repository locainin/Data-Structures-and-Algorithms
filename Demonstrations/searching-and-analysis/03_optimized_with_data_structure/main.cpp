// Demonstration: searching and analysis (03_optimized_with_data_structure)
// Purpose: compare baseline and optimized behavior under the same workload

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kItemCount = 30000;
    constexpr int kQueryCount = 4000;

    std::vector<int> customerIds;
    customerIds.reserve(kItemCount);

    // Data is ordered so binary search can split the range each step
    for (int i = 0; i < kItemCount; i++) {
        customerIds.push_back(i * 2);
    }

    std::mt19937 rng(45);
    std::uniform_int_distribution<int> queryDist(0, kItemCount * 2);

    int hits = 0;
    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Query count matches baseline for an apples-to-apples comparison
    for (int i = 0; i < kQueryCount; i++) {
        // binary_search uses ordered range partitioning
        if (std::binary_search(customerIds.begin(), customerIds.end(), queryDist(rng))) {
            hits++;
        }
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Optimized Customer Lookup\n";
    std::cout << "hits: " << hits << '/' << kQueryCount << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
