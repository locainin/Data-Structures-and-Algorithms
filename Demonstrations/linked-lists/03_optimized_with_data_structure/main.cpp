// Demonstration: linked lists (03_optimized_with_data_structure)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <list>
#include <random>
#include <string>

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kInitialTracks = 7000;
    constexpr int kOperations = 3000;

    std::list<std::string> playlist;
    // Initial dataset mirrors baseline size
    for (int i = 0; i < kInitialTracks; i++) {
        playlist.push_back("track_" + std::to_string(i));
    }

    std::mt19937 rng(44);
    std::uniform_int_distribution<int> jumpDist(1, 8);
    auto current = playlist.begin();

    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Perform localized edits near current iterator position
    for (int i = 0; i < kOperations; i++) {
        // Small localized edits around current song are cheap with list links
        for (int j = 0; j < jumpDist(rng); j++) {
            current++;
            if (current == playlist.end()) {
                current = playlist.begin();
            }
        }

        current = playlist.insert(current, "new_track_" + std::to_string(i));

        auto toErase = std::next(current);
        if (toErase == playlist.end()) {
            toErase = playlist.begin();
        }

        if (toErase != playlist.end()) {
            playlist.erase(toErase);
        }
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Optimized Playlist Editing\n";
    std::cout << "final size: " << playlist.size() << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
