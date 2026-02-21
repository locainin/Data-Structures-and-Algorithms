// Demonstration: hash tables (03_optimized_with_data_structure)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kTokenCount = 8000;
    constexpr int kQueryCount = 6000;

    // Hash table stores active tokens for constant-time average lookup
    std::unordered_set<std::string> tokenSet;
    tokenSet.reserve(static_cast<size_t>(kTokenCount * 2));

    for (int i = 0; i < kTokenCount; i++) {
        tokenSet.insert("token_" + std::to_string(i));
    }

    std::mt19937 rng(43);
    std::uniform_int_distribution<int> tokenDist(0, kTokenCount * 2);

    int found = 0;
    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Same query shape as baseline, but lookup path is hashed
    for (int i = 0; i < kQueryCount; i++) {
        if (tokenSet.contains("token_" + std::to_string(tokenDist(rng)))) {
            found++;
        }
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Optimized Token Validation\n";
    std::cout << "found tokens: " << found << '/' << kQueryCount << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
