// Demonstration: hash tables (02_unoptimized)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

bool containsTokenLinear(const std::vector<std::string>& tokens, const std::string& query) {
    // Linear probe through every token entry
    for (const auto& token : tokens) {
        if (token == query) {
            return true;
        }
    }
    return false;
}

} // namespace

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kTokenCount = 8000;
    constexpr int kQueryCount = 6000;

    std::vector<std::string> tokens;
    tokens.reserve(kTokenCount);
    // Build synthetic token pool used by incoming requests
    for (int i = 0; i < kTokenCount; i++) {
        tokens.push_back("token_" + std::to_string(i));
    }

    std::mt19937 rng(43);
    std::uniform_int_distribution<int> tokenDist(0, kTokenCount * 2);

    int found = 0;
    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Simulate repeated token checks for mixed hit/miss traffic
    for (int i = 0; i < kQueryCount; i++) {
        if (containsTokenLinear(tokens, "token_" + std::to_string(tokenDist(rng)))) {
            found++;
        }
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Unoptimized Token Validation\n";
    std::cout << "found tokens: " << found << '/' << kQueryCount << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
