#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <type_traits>

#include <ppp/concurrent/Monitor.hpp>


static_assert(not std::is_same_v<ppp::Monitor<>, ppp::Monitor<>>);


auto main() -> int {
    enum class Condition { A, B, C, None };
    using enum Condition;

    auto state = None;
    auto monitor = ppp::Monitor<A, B, C> {};

    auto threadA = std::jthread([&monitor, &state] {
        auto lock = monitor.getLock();
        std::cout << "Thread A: waiting for condition A" << std::endl;
        while (state != A) {
            monitor.wait<A>(lock);
        }
        switch (state) {
        case A:
            std::cout << "Thread A: condition A met" << std::endl;
            return;
        default:
            throw std::runtime_error { "Thread A: unexpected state" };
        }
    });

    auto threadB = std::jthread([&monitor, &state] {
        auto lock = monitor.getLock();
        std::cout << "Thread B: waiting for condition B" << std::endl;
        while (state != B) {
            monitor.wait<B>(lock);
        }
        switch (state) {
        case B:
            std::cout << "Thread B: condition B met" << std::endl;
            return;
        default:
            throw std::runtime_error { "Thread B: unexpected state" };
        }
    });

    auto threadC = std::jthread([&monitor, &state] {
        auto lock = monitor.getLock();
        std::cout << "Thread C: waiting for condition C" << std::endl;
        while (state != C) {
            monitor.wait<C>(lock);
        }
        switch (state) {
        case C:
            std::cout << "Thread C: condition C met" << std::endl;
            return;
        default:
            throw std::runtime_error { "Thread C: unexpected state" };
        }
    });

    using namespace std::chrono_literals;

    std::this_thread::sleep_for(1s);
    std::cout << "Main thread: signaling condition B" << std::endl;
    state = B;
    monitor.signal<B>();

    std::this_thread::sleep_for(1s);
    std::cout << "Main thread: signaling condition C" << std::endl;
    state = C;
    monitor.signal<C>();

    std::this_thread::sleep_for(1s);
    std::cout << "Main thread: signaling condition A" << std::endl;
    state = A;
    monitor.signal<A>();

    return 0;
}
