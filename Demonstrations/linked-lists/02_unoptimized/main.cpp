// Demonstration: linked lists (02_unoptimized)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kInitialTracks = 7000;
    constexpr int kOperations = 3000;

    std::vector<std::string> playlist;
    playlist.reserve(static_cast<size_t>(kInitialTracks + kOperations));

    // Create starting playlist snapshot
    for (int i = 0; i < kInitialTracks; i++) {
        playlist.push_back("track_" + std::to_string(i));
    }

    std::mt19937 rng(44);
    std::uniform_int_distribution<int> posDist(0, kInitialTracks - 1);

    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Random mid-list edits model user navigation and edits
    for (int i = 0; i < kOperations; i++) {
        const int rawPos = posDist(rng);
        const size_t pos = static_cast<size_t>(rawPos) % (playlist.size() + 1);

        // Vector insert in middle shifts many elements on average
        playlist.insert(playlist.begin() + static_cast<std::ptrdiff_t>(pos), "new_track_" + std::to_string(i));

        // Matching middle erase creates another shifting cost
        const size_t erasePos = (pos + 1) % playlist.size();
        playlist.erase(playlist.begin() + static_cast<std::ptrdiff_t>(erasePos));
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Unoptimized Playlist Editing\n";
    std::cout << "final size: " << playlist.size() << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
