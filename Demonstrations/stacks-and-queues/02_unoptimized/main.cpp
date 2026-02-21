// Demonstration: stacks and queues (02_unoptimized)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kInitialTickets = 9000;
    constexpr int kOperations = 5000;

    std::vector<std::string> ticketQueue;
    ticketQueue.reserve(static_cast<size_t>(kInitialTickets + kOperations));

    // Seed initial queue backlog
    for (int i = 0; i < kInitialTickets; i++) {
        ticketQueue.push_back("ticket_" + std::to_string(i));
    }

    std::mt19937 rng(47);
    std::uniform_int_distribution<int> newTicketDist(0, 900000);

    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Simulate FIFO processing with front-removal on vector
    for (int i = 0; i < kOperations; i++) {
        // Expensive queue pop from front because all remaining items shift left
        if (!ticketQueue.empty()) {
            ticketQueue.erase(ticketQueue.begin());
        }

        ticketQueue.push_back("ticket_" + std::to_string(newTicketDist(rng)));
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Unoptimized Ticket Queue\n";
    std::cout << "remaining tickets: " << ticketQueue.size() << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
