// Demonstration: trees (03_optimized_with_data_structure)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <string>

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kProductCount = 25000;
    constexpr int kQueryCount = 5000;

    std::map<int, std::string> products;

    // Ordered inserts build balanced tree nodes by key
    for (int i = 0; i < kProductCount; i++) {
        products.emplace(i * 3, "product_" + std::to_string(i));
    }

    std::mt19937 rng(48);
    std::uniform_int_distribution<int> idDist(0, kProductCount * 3);

    int hits = 0;
    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Exact key lookups use tree navigation
    for (int i = 0; i < kQueryCount; i++) {
        if (products.find(idDist(rng)) != products.end()) {
            hits++;
        }
    }

    // Tree structure supports ordered range queries with bounded traversal
    int rangeCount = 0;
    const int rangeStart = 12000;
    const int rangeEnd = 13000;
    auto it = products.lower_bound(rangeStart);
    while (it != products.end() && it->first <= rangeEnd) {
        rangeCount++;
        it++;
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Optimized Product Lookup\n";
    std::cout << "hits: " << hits << '/' << kQueryCount << '\n';
    std::cout << "range_count_" << rangeStart << '_' << rangeEnd << ": " << rangeCount << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
