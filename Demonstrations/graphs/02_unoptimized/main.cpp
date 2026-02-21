// Demonstration: graphs (02_unoptimized)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <utility>
#include <vector>

namespace {

bool canReachByScanningEdges(int source, int target, int nodeCount,
                             const std::vector<std::pair<int, int>> &edges) {
  std::vector<bool> visited(static_cast<size_t>(nodeCount), false);
  std::queue<int> pending;

  visited[source] = true;
  pending.push(source);

  while (!pending.empty()) {
    const int current = pending.front();
    pending.pop();

    if (current == target) {
      return true;
    }

    // Intentionally expensive neighbor lookup by scanning all edges every step
    for (const auto &edge : edges) {
      if (edge.first != current) {
        continue;
      }

      if (!visited[edge.second]) {
        visited[edge.second] = true;
        pending.push(edge.second);
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

    std::vector<std::pair<int, int>> edges;
    edges.reserve(kEdgeCount);

    // Generate synthetic directed road links
    for (int i = 0; i < kEdgeCount; i++) {
        edges.emplace_back(nodeDist(rng), nodeDist(rng));
    }

    int reachableCount = 0;
    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Repeated source/target checks expose the repeated edge-scan cost
    for (int i = 0; i < kQueryCount; i++) {
        if (canReachByScanningEdges(nodeDist(rng), nodeDist(rng), kNodeCount, edges)) {
            reachableCount++;
        }
  }

  const auto end = std::chrono::steady_clock::now();
  const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  std::cout << "Unoptimized Graph Reachability\n";
  std::cout << "reachable queries: " << reachableCount << '/' << kQueryCount << '\n';
  std::cout << "elapsed_ms: " << elapsedMs << '\n';
  return 0;
}
