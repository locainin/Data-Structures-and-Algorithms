// Demonstration: graphs (03_optimized_with_data_structure)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

namespace {

bool canReachWithAdjacencyList(int source, int target, const std::vector<std::vector<int>> &adjacency) {
  std::vector<bool> visited(adjacency.size(), false);
  std::queue<int> pending;

  visited[static_cast<size_t>(source)] = true;
  pending.push(source);

  while (!pending.empty()) {
    const int current = pending.front();
    pending.pop();

    if (current == target) {
      return true;
    }

    // Fast neighbor access through pre-built adjacency list
    for (const int next : adjacency[static_cast<size_t>(current)]) {
      if (!visited[static_cast<size_t>(next)]) {
        visited[static_cast<size_t>(next)] = true;
        pending.push(next);
      }
    }
  }

  return false;
}

} // namespace

int main() {
  // Benchmark constants keep runtime practical for local walkthroughs
  constexpr int kNodeCount = 1800;
  constexpr int kEdgeCount = 18000;
  constexpr int kQueryCount = 700;

  std::mt19937 rng(42);
  std::uniform_int_distribution<int> nodeDist(0, kNodeCount - 1);

    std::vector<std::vector<int>> adjacency(static_cast<size_t>(kNodeCount));

    // Build adjacency list once before answering queries
    for (int i = 0; i < kEdgeCount; i++) {
        const int from = nodeDist(rng);
        const int to = nodeDist(rng);
        adjacency[static_cast<size_t>(from)].push_back(to);
    }

    int reachableCount = 0;
    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Query pattern matches the baseline, but neighbor access is direct
    for (int i = 0; i < kQueryCount; i++) {
        if (canReachWithAdjacencyList(nodeDist(rng), nodeDist(rng), adjacency)) {
            reachableCount++;
        }
  }

  const auto end = std::chrono::steady_clock::now();
  const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  std::cout << "Optimized Graph Reachability\n";
  std::cout << "reachable queries: " << reachableCount << '/' << kQueryCount << '\n';
  std::cout << "elapsed_ms: " << elapsedMs << '\n';
  return 0;
}
