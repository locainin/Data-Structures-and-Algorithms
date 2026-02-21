// Demonstration: stacks and queues (03_optimized_with_data_structure)
// Purpose: compare baseline and optimized behavior under the same workload

#include <chrono>
#include <deque>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main() {
    // Benchmark constants keep runtime practical for local walkthroughs
    constexpr int kInitialTickets = 9000;
    constexpr int kOperations = 5000;

    std::deque<std::string> ticketQueue;
    std::vector<std::string> actionStack;
    actionStack.reserve(static_cast<size_t>(kOperations));

    // Seed initial queue backlog
    for (int i = 0; i < kInitialTickets; i++) {
        ticketQueue.push_back("ticket_" + std::to_string(i));
    }

    std::mt19937 rng(47);
    std::uniform_int_distribution<int> newTicketDist(0, 900000);

    // Measure only the hot-path work to compare both versions fairly
    const auto start = std::chrono::steady_clock::now();

    // Simulate queue processing plus occasional undo stack updates
    for (int i = 0; i < kOperations; i++) {
        // O(1) style queue behavior with deque front operations
        if (!ticketQueue.empty()) {
            actionStack.push_back(ticketQueue.front());
            ticketQueue.pop_front();
        }

        ticketQueue.push_back("ticket_" + std::to_string(newTicketDist(rng)));

        // Stack model for simple undo tracking
        if (i % 25 == 0 && !actionStack.empty()) {
            actionStack.pop_back();
        }
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Optimized Ticket Queue\n";
    std::cout << "remaining tickets: " << ticketQueue.size() << '\n';
    std::cout << "undo stack size: " << actionStack.size() << '\n';
    std::cout << "elapsed_ms: " << elapsedMs << '\n';
    return 0;
}
