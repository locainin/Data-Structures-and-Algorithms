// Demonstration: trees (02_unoptimized)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace {

bool linearContains(const std::vector<std::pair<int, std::string>>& products, int productId) {
    // Baseline model checks each product id one-by-one
    for (const auto& product : products) {
        if (product.first == productId) {
            return true;
        }
    }
    return false;
}

} // namespace

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kProductCount = 25000;
    constexpr int kQueryCount = 5000;

    std::vector<std::pair<int, std::string>> products;
    products.reserve(kProductCount);

    // Deterministic id spacing keeps hit ratio stable between runs
    for (int i = 0; i < kProductCount; i++) {
        products.emplace_back(i * 3, "product_" + std::to_string(i));
    }

    std::mt19937 rng(48);
    std::uniform_int_distribution<int> idDist(0, kProductCount * 3);

    int hits = 0;
    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Repeat lookup queries to surface scaling cost of linear search
    for (int i = 0; i < kQueryCount; i++) {
        if (linearContains(products, idDist(rng))) {
            hits++;
        }
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Unoptimized Product Lookup\n";
    std::cout << "hits: " << hits << '/' << kQueryCount << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
