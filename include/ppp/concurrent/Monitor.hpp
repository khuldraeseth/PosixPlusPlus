#pragma once

#include <condition_variable>
#include <mutex>


namespace ppp {

namespace detail {

template <auto id, auto... cs>
class Monitor {};

template <auto id>
class Monitor<id> {
public:
    struct Mutex : public std::mutex {};
    Mutex mutex {};

    auto getLock() -> std::unique_lock<Mutex> { return std::unique_lock { mutex }; }
};

template <auto id, auto c, auto... Cs>
class Monitor<id, c, Cs...> : public Monitor<id, Cs...> {
    std::condition_variable cv {};
    using Base = Monitor<id, Cs...>;

public:
    using Mutex = typename Base::Mutex;
    using Base::getLock;

    template <auto c2>
    auto wait(std::unique_lock<Mutex>& lock) -> void {
        if constexpr (c == c2) {
            auto boringLock = std::unique_lock(static_cast<std::mutex&>(*lock.release()), std::adopt_lock);
            cv.wait(boringLock);
            lock = std::unique_lock(static_cast<Mutex&>(*boringLock.release()), std::adopt_lock);
        } else {
            Base::template wait<c2>(lock);
        }
    }

    template <auto c2>
    auto signal() -> void {
        if constexpr (c == c2) {
            cv.notify_one();
        } else {
            Base::template signal<c2>();
        }
    }

    template <auto c2>
    auto broadcast() -> void {
        if constexpr (c == c2) {
            cv.notify_all();
        } else {
            Base::template broadcast<c2>();
        }
    }
};

}   // namespace detail

// A monitor is a mutex with a collection of associated condition variables.
// `Monitor` is parameterized by a list of condition identifiers. These can
//  be enumerators; that's what I had in mind when writing this.
// The lambda expression here is a neat little C++20 trick to cause each name
//  written `Monitor<cs...>` to name a distinct type, even if `cs` is the same.
// Why? Because it doesn't make sense to wait on one monitor with a lock on
//  another.
// GCC does not handle this properly. It treats the lambda expression as
//  naming the same object in each instantiation.
// Fortunately, clang has the behavior I want! So I warn below if clang is not
//  detected.
template <auto... cs>
using Monitor = detail::Monitor<[] {}, cs...>;

#ifndef __clang__
#pragma message(                                                                                \
  "This code is written for clang. It's untested with other compilers. Use at your own risk.\n" \
  "In particular, GCC 12.2.1 has a known misbehavior that allows certain bugs.")
#endif

}   // namespace ppp
